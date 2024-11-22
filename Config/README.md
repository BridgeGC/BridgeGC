## Config Flink Applications
For Flink applications, the datasets are produced through the 
generators in its official 
[repository](https://github.com/apache/flink/blob/release-1.9/flink-examples), 
including [KMeans Generator](https://github.com/apache/flink/blob/release-1.9/flink-examples/flink-examples-batch/src/main/java/org/apache/flink/examples/java/clustering/util/KMeansDataGenerator.java),
[Linear Regression Generator](https://github.com/apache/flink/blob/release-1.9/flink-examples/flink-examples-batch/src/main/java/org/apache/flink/examples/java/ml/util/LinearRegressionDataGenerator.java),
[Web Log Generator](https://github.com/apache/flink/blob/release-1.9/flink-examples/flink-examples-batch/src/main/java/org/apache/flink/examples/java/relational/util/WebLogDataGenerator.java).
We use part of [Twitter social graph](http://an.kaist.ac.kr/traces/WWW2010.html) for PageRank and ConnectedComponents.


The size of Flink data sets are Linear Regression (LR,
800 million points), KMeans (KM, 600 centers and 240 million points), PageRank (PR, 40 million
nodes and 385 million edges), WebLogAnalysis (WA, 50 million documents and 100 million visits).

When we evaluate the on-heap strategy, all memory budget (e.g., 32 GB) is used for the JVM heap of the task manager,
with the default 70% used as managed memory. 
When we evaluate the off-heap strategy, we set the `taskmanager.memory.off-heap`
to true, and the default
30% memory budget (e.g., 9.6 GB) is used for the JVM heap of the task manager, and the left 70%
(e.g., 22.4 GB) memory budget for managed memory is allocated off-heap.

## Config Spark Applications
For Spark applications, the datasets are produced through the generators
provided by [HiBench](https://github.com/Intel-bigdata/HiBench), 
whose sizes can be set in the configuration files of applications.

The size of Spark data sets are Linear Regression (LR, 240
thousand points and 20 thousand features), Support Vector Machine (SVM, 120 thousand points and
100 thousand features), Gaussian Mixture Model (GMM, 240 million points and 15 centers), KMeans
(KM, 240 million points and 10 centers), and PageRank (PR, 7 million pages).

When we evaluate the on-heap strategy, we use the `memory_and_disk_ser` storage
level, and all memory budget (e.g., 32 GB) is used for the JVM heap of the executor, with the default
60% (e.g., 19.2 GB) used for storage memory and execution memory. When we evaluate the off-heap
strategy, we use the `off_heap` storage level, set true the `spark.memory.offheap.enabled`, and
use the default 40% memory (e.g., 12.8 GB) for the JVM heap of the executor, and the left 60% (e.g.,
19.2 GB) memory for storage and execution is allocated off-heap.

## Config Cassandra Applications
For Cassandra applications, the datasets are generated through [YCSB](https://github.com/brianfrankcooper/YCSB), 
whose sizes can be set in the workload files of workloads.

The size of Cassandra data sets are write-intensive
workload (WI, 3 million records with 2.4 million updates and 0.6 million reads), and read-intensive
workload (RI, 3 million records with 0.6 million updates and 2.4 million reads).

When we evaluate the on-heap strategy, we adopt the `unslabbed_heap_buffers`
implementation of Memetable, all memory budget (e.g., 12 GB) is used for the JVM heap of the
server, and we set the threshold for `memtable_heap_space` to 50% of the heap memory (e.g., 6
GB). When we evaluate the off-heap strategy, we adopt the `offheap_objects` implementation
of Memetable, using 75% memory budget (e.g., 9 GB) for the JVM heap of the server, and set
the threshold for both `memtable_heap_space` and `memtable_offheap_space` to 25% of the heap
memory (e.g., 3 GB).