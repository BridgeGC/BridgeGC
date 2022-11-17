# BridgeGC

Existing garbage collectors usually suffer from heavy GC overhead for large-scale data processing. The primary cause is the gap between big data frameworks and garbage collectors --- big data frameworks usually generate massive long-lived objects, while existing garbage collectors are designed based on the hypothesis that most objects live short. BridgeGC is a semi-automatic garbage collector built on OpenJDK 17 HotSpot to bridge this gap, which especially aiming at reducing GC time of long-lived data objects.

# Usage

## Build

Download the [source](./) of BridgeGC-OpenJDK17-HotSpot, follow the [instructions](./) to build the JDK.

## Download

[BridgeGC-OpenJDK17-HotSpot-source](./)

[BridgeGC-OpenJDK17-HotSpot-Linux-x64](./)

[Spark-3.3.0-source](./)

[Flink-1.14.0-source](./)

[Cassandra](./)

## Spark

## Flink

```java
public class MemorySegment {
  protected final byte[] heapMemory;
}

pubic MemorySegment allocateSegment(int size) {
  // Hint for identification
  byte[] backup = new @Long byte[size];
  MemorySegment res = new @Long MemorySegment(backup);
  return res;
}

void release(Collection<MemorySegment> segments) {
  while (segments.hasNext()) {
    final MemorySegment seg = segments.next();
    seg.free();
  }
  segments.clear();
  // Hint for reclamation
  System.Deadpoint();
}
```

## Cassandra

# Results
