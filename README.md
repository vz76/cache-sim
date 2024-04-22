# Cache Simulator for CS378 Energy Efficient Computing

## Execution

To run in terminal: 
```
chmod +x ./compile.sh ./run.sh && ./run.sh
```

## Overview

Summary output metrics can be found in the `./outputs` folder, with mean / stdev calculations for energy and time consumption at each set associativity level. 

More detailed logs can be found in the `./logs` folder, which contains metrics pertaining to cache hits and misses for L1 and L2 for each run.

Our methodology of tracking metrics is explained further in `./REPORT.pdf`, which references the tables (stats) found in the `./outputs` folder. Reasonable assumptions are made given the original spec which might not conform to the new spec, as allowed in [Ed #151](https://edstem.org/us/courses/54035/discussion/4804452), which may explain some deviances in these metrics.