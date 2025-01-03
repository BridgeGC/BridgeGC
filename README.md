# BridgeGC
BridgeGC is an efficient cross-level garbage collector for big data frameworks such as Flink and Spark. Specially, BridgeGC is built on [OpenJDK 17 HotSpot](https://github.com/openjdk/jdk17) to reduce the GC time spent on long-lived data objects generated by the frameworks. As shown in Figure 1, BridgeGC uses limited manual annotations at the creation/release points of data objects to profile their life cycles, then BridgeGC leverages the life cycles information of annotated data objects to efficiently allocate them in JVM heap and reclaim them without unnecessary marking/copying overhead.
<div align=center>
<img decoding="async" src="Figures/bridgegc-intro.svg" width="50%">

**Figure 1: Comparison between BridgeGC and traditional GC.**
</div>

# Usage

## Build

Download our source code, follow the [instructions](https://openjdk.org/groups/build/doc/building.html) to build the OpenJDK JVM, resulting in a JVM with BridgeGC. 

## Annotations
We provide two simple annotations, `@DataObj` and `System.Reclaim()`, 
that can be used by the framework developers to annotate the creation 
and release points of data objects. 
Specifically, the annotation `@DataObj` is used 
along with the keyword `new` to denote the creation of data objects. 
The annotation `System.Reclaim()` is used to denote the 
release of a batch of data objects. 
We show how we apply annotations in Spark, Flink 
and Cassandra briefly as follows and more details 
can be found [here](Apply/README.md).

<div align=center>
<img decoding="async" src="Figures/flink.svg" width="50%">

**Figure 2: Apply BridgeGC annotations to `MemorySegment`.**
</div>

<div align=center>
<img decoding="async" src="Figures/spark-rdd.svg" width="50%">

**Figure 3: Apply BridgeGC annotations to chunked `ByteBuffer`.**
</div>

<div align=center>
<img decoding="async" src="Figures/spark-tungsten.svg" width="50%">

**Figure 4: Apply BridgeGC annotations to `MemoryBlock`.**
</div>

<div align=center>
<img decoding="async" src="Figures/BufferCell.svg" width="50%">

**Figure 5: Apply BridgeGC annotations to `BufferCell` for Memtable.**
</div>

## Run
After adding the annotations, compile the framework. Before running the framework, just add `-XX:+UseBridgeGC` to JVM parameters of the executor/server to enable BridgeGC.

# Implementation
We design three components in BridgeGC to efficiently profile, allocate, and reclaim annotated data objects.

## Precise Data Object Profiler
The profiler is designed to identify data objects and track the life cycles of data objects through annotations, it processes `@DataObj` and `System.Reclaim()` annotations at the runtime to inform the garbage collector of allocation and reclaimable time of data objects.

## Memory-Efficient Label-Based Allocator
The allocator separates the storage of data objects and normal objects in data pages and normal pages, and tackles the problem of space balance by dynamic page allocation. To distinguish data objects readily at the GC level, the allocator labels them using colored pointer.

## Effective Marking/Copying-Conservation Collector 
The collector skips marking labeled data objects and excludes data pages from reclamation in GC cycles where data objects are known to be lived, and performs effective reclamation of data pages only after some annotated data objects are released at the framework level.

# Evaluation
We apply and evaluate BridgeGC with Flink 1.9.3, Spark 3.3.0 and Cassandra 4.0.6. We compare BridgeGC with all available garbage collectors in OpenJDK 17, includes ZGC, G1, Shenandoah and Parallel. 
<!-- We also compare BridgeGC with a state-of-the-art research work [ROLP](https://rodrigo-bruno.github.io/papers/rbruno-eurosys19.pdf).-->

For Flink, we select five batch machine learning applications that are memory 
intensive from [Flink examples](https://github.com/apache/flink/tree/master/flink-exa) 
as the driving workload, including Linear Regression (LR), KMeans (KM), PageRank (PR), 
Components (CC) and WebLogAnalysis (WA). 
For Spark, we choose five representative ML applications 
from popular big data benchmark [HiBench](https://github.com/Intel-bigdata/HiBench), 
including Linear Regression (LR), Support Vector Machine (SVM), 
Gaussian Mixture Model (GMM), PageRank (PR) and KMeans (KM).
For Cassandra, we choose two workloads from a popular NoSQL database benchmark YCSB, 
including Write-Intensive (WI) workload and Read-Intensive (RI) workload.
The detail we config the application and frameworks can be found [here](Config/README.md).

<div align=center>
<img decoding="async" src="Figures/taco_gc_modified.svg" width="80%">

**Figure 6: The total concurrent marking and copying GC time that BridgeGC and ZGC 
spend when running applications with different heap sizes.**
</div>

As the results shown in Figure 6, BridgeGC reduces concurrent GC time by 42\%-82\% compared to baseline ZGC. 
BridgeGC achieves lower GC time by consuming 7\%-13\% less memory than ZGC and 
having 2\%-52\% fewer GC cycle counts. 
Also, BridgeGC spends 31\%-46\% less marking time per GC cycle. 

<div align=center>
<img decoding="async" src="Figures/taco_exec.JPG" width="55%">

**Figure 7: Execution time of applications under baseline ZGC and execution time of other collectors normalized to ZGC.**
</div>

In terms of application execution time, 
BridgeGC outperforms other evaluated collectors for 
all workloads and configurations as shown in Figure 7. 
Compared to the baseline ZGC, BridgeGC achieves 3%-29% speedup. 
BridgeGC also reduces up to 26% execution time compared to 
the default collector G1 in OpenJDK.
As shown in Figure 8, BridgeGC also outperforms all evaluated collectors
in latency metrics.
BridgeGC improves applications’ performance 
mainly due to fewer GC cycles and less GC overhead.   

<div align=center>
<img decoding="async" src="Figures/taco_latency.JPG" width="65%">

**Figure 8: Latency metrics of Cassandra workloads under evaluated collectors.**
</div>