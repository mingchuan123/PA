/*
  This file implement an tagged cache stride prefetcher

 */

#include <stdio.h>
#include "../inc/prefetcher.h"

#define CACHE_TRACKER_COUNT 1024
#define PREFETCH_DEGREE 2


//define a cache_tracker for cache line 
typedef struct cache_tracker
{
     unsigned char tag;
     unsigned int addr;
     unsigned int lru;
     short int stride;
     unsigned char conf;
} cache_tracker_t;

cache_tracker_t trackers[CACHE_TRACKER_COUNT];


//initialization for cache_tracker 
void l2_prefetcher_initialize(int cpu_num)
{
  printf("Tagged Astride Prefetching\n");
 
  int i;
  for(i=0;i<CACHE_TRACKER_COUNT;i++)
     {
        trackers[i].tag = 0;
        trackers[i].addr = 0; 
        trackers[i].lru = 0;
        trackers[i].stride=0;
        trackers[i].conf=0;
     }
}
 
//operation on cache_tracker
void l2_prefetcher_operate(int cpu_num, unsigned long long int addr, unsigned long long int ip, int cache_hit)
{
   //int stride=0;
   int index=-1;
   int str_index=0;
   //find the same address on the cache line
   int i;
   for(i=0;i<CACHE_TRACKER_COUNT;i++)
      {
         if(trackers[i].addr==addr)
           {  
              index=i;
              trackers[index].tag=0;
              trackers[index].lru=get_current_cycle(0);
              //trackers[index].stride=0;
              break;
           }
      }
         
    //a brand new address
    if(index==-1)
      {
          int lru_index=0;
          int last_index=0;
          int second_index=0;
          int sec_str_index=0;
          unsigned long long int lru_cycle=trackers[0].lru;
          //find the least recent used one 
          for(i=0;i<CACHE_TRACKER_COUNT;i++)
             {
                if(trackers[i].lru<lru_cycle)
                  {
                     lru_index=i;
                     lru_cycle=trackers[lru_index].lru; 
                   }
                
              }
            
          index=lru_index;
               
          //find the most recent used one   
          for(i=0;i<CACHE_TRACKER_COUNT;i++)
             {
                if(trackers[i].lru>lru_cycle) 
                   {
                      last_index=i;
                      lru_cycle=trackers[last_index].lru; 
                   }
             }
          str_index=last_index;

          //find the second most recent used one
          for(i=0;i<CACHE_TRACKER_COUNT;i++)
             {
                if((lru_cycle<trackers[i].lru)&&(trackers[i].lru<trackers[str_index].lru))
                  {
                     second_index=i;
                     lru_cycle=trackers[second_index].lru;
                  }
             }
          sec_str_index=second_index;  
                              
          //reset the lru one
          trackers[index].tag=1;
          trackers[index].addr=addr;
          trackers[index].lru=get_current_cycle(0);
          trackers[index].stride=trackers[index].addr-trackers[str_index].addr;
          
          //confidence additon
          if(trackers[index].stride==trackers[str_index].stride)
             {
                   trackers[index].conf=trackers[str_index].conf+1;
             }
          else
             {   
                if(trackers[index].stride==trackers[sec_str_index].stride)
                  {
                     trackers[str_index].conf=2;
                  }      
             }
       }
          

    //no new fetched address
    if(trackers[index].tag==0)
     {
         return;

     }
    //new fetched address and prefetch for three times
    else
     {   
          int stride=0;
          if(trackers[index].conf==2)
            {
                stride=trackers[index].stride;
                trackers[index].conf=trackers[index].conf-1;
            }
          if(trackers[str_index].conf==2)
            {
                stride=trackers[str_index].stride;
                trackers[str_index].conf=0;
            }
          
          else
            {
                stride=1;
            } 
           
          for(i=0;i<PREFETCH_DEGREE;i++)
             {  
                unsigned long long int pf_addr=((addr>>6)+i*stride)<<6;     
                
                if((pf_addr>>12) != (addr>>12))
                  {
                    break;
                  }  
                
                // check the MSHR occupancy to decide if we're going to prefetch in L2 or LLC
                if(get_l2_mshr_occupancy(0)<8) 
                  {    
                      l2_prefetch_line(0, addr, pf_addr, FILL_L2);
                  }
                 else
                  {
                      l2_prefetch_line(0, addr, pf_addr, FILL_LLC);
                  }
             }
     }

}



void l2_cache_fill(int cpu_num, unsigned long long int addr, int set, int way, int prefetch, unsigned long long int evicted_addr)
{
  
}

void l2_prefetcher_heartbeat_stats(int cpu_num)
{
  printf("Prefetcher heartbeat stats\n");
}

void l2_prefetcher_warmup_stats(int cpu_num)
{
  printf("Prefetcher warmup complete stats\n\n");
}

void l2_prefetcher_final_stats(int cpu_num)
{
  printf("Prefetcher final stats\n");
}
