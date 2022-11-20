# BridgeGC

BridgeGC is a semi-automatic garbage collector built on OpenJDK 16 HotSpot aiming at reducing GC time of long-lived data objects.

For more details, please refer to out [paper](./).

# Usage

## Build & Run

Download the [source](./) of BridgeGC-OpenJDK16-HotSpot, follow the [instructions](./) to build the JDK, and use it like OpenJDK.

## Use BridgeGC

BridgeGC accurately manage long-lived objects with hints added manually.

To make BridgeGC effectual, hints should be applied at the framework level to data objects with features of long-lived, great amount, and a simple reference relationship. Abstracted data objects, for example [MemorySegment](https://github.com/apache/flink/blob/master/flink-core/src/main/java/org/apache/flink/core/memory/MemorySegment.java) in [Flink](http://flink.apache.org/), generally satisfy these features, and they are widely used in modern big data frameworks.

```java
public class MemorySegment {
  // Simple reference relationship
  protected final byte[] heapMemory;
}
```

MemorySegment is explicitly created and released in fixed functions, hints(annotation `@Long` and function `Deadpoint`) could be easily applied in these functions.

BridgeGC identifies target data objects and manages their creation by leveraging hints at their allocation sites. The annotation `@Long` for keyword `new` is added to the codes of creation functions where data objects are allocated. During object allocation at GC level, BridgeGC determines a memory request as an allocation of data object if it is annotated, and places the object into a special region.

```java
pubic MemorySegment allocateSegment(int size) {
  // Hint for identification
  byte[] backup = new @Long byte[size];
  MemorySegment res = new @Long MemorySegment(backup);
  return res;
}
```

To realize the life cycles of all data objects and reclaim them, hints at their release function signal common dead points. The Java System Call `System.Deadpoint()` is added in the release functions after all data objects are explicitly released by frameworks. BridgeGC would determine some data objects have finished their life cycles and become reclaimable after receiving a signal of `System.Deadpoint()`.

```java
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

# Results
