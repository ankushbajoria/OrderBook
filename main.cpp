#include "FeedHandler.h"

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/tail_quantile.hpp>
#include <boost/accumulators/statistics/max.hpp>
#include <boost/accumulators/statistics/min.hpp>


#include <ios>
#include <fstream>
#include <string>
#include <complex>
#include <chrono>

#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>

static __inline__ unsigned long long rdtsc(void)
{
  unsigned hi, lo;
  __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
  return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}

int main(int argc, char** argv)
{
  FeedHandler feed;

  std::string line;
  const std::string filename(argv[1]);
  std::ifstream infile(filename.c_str(), std::ios::in);

  int counter = 0;

  using min       = boost::accumulators::tag::min;
  using max       = boost::accumulators::tag::max;
  using mean      = boost::accumulators::tag::mean;
  using quantile  = boost::accumulators::tag::tail_quantile<boost::accumulators::right>;
  using Features  = boost::accumulators::stats< min, max, mean, quantile>;

  using namespace boost::accumulators;
  std::size_t index_ = 100000;
  boost::accumulators::accumulator_set<uint64_t, Features> latencies(tag::tail<right>::cache_size = index_);

  while (getline(infile, line))
  {
    auto now = rdtsc();
    feed.processMessage(line);
    auto end = rdtsc();

    auto diff = end - now;
    latencies(diff);

    counter++;

#if 0
    if (counter % 10 == 0) {
      feed.printCurrentOrderBook(std::cout);
    }
#endif
  }

  std::cout << "Min latency:  " << boost::accumulators::min(latencies)    << std::endl;
  std::cout << "Max latency:  " << boost::accumulators::max(latencies)    << std::endl;
  std::cout << "Mean latency: " << boost::accumulators::mean(latencies)   << std::endl;

  std::cout << "Quantile .01: " << boost::accumulators::quantile(latencies, quantile_probability = 0.01) << std::endl;
  std::cout << "Quantile .05: " << boost::accumulators::quantile(latencies, quantile_probability = 0.05) << std::endl;
  std::cout << "Quantile .10: " << boost::accumulators::quantile(latencies, quantile_probability = 0.1) << std::endl;
  std::cout << "Quantile .25: " << boost::accumulators::quantile(latencies, quantile_probability = 0.25) << std::endl;
  std::cout << "Quantile .50: " << boost::accumulators::quantile(latencies, quantile_probability = 0.5) << std::endl;
  std::cout << "Quantile .75: " << boost::accumulators::quantile(latencies, quantile_probability = 0.75) << std::endl;
  std::cout << "Quantile .90: " << boost::accumulators::quantile(latencies, quantile_probability = 0.90) << std::endl;
  std::cout << "Quantile .95: " << boost::accumulators::quantile(latencies, quantile_probability = 0.95) << std::endl;
  std::cout << "Quantile .99: " << boost::accumulators::quantile(latencies, quantile_probability = 0.99) << std::endl;

  return 0;
}
