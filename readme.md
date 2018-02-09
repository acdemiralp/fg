**What is a framegraph?**
A rendering abstraction which describes a frame as a directed acyclic graph of render tasks and resources.
  
**What is a render task?**
A compute or graphics task to be performed as part of a rendering pipeline.

**What is a resource?**
Data created, read or written by a render task. Alternates between two states; virtual and real.
While virtual, the resource is not instantiated but contains the necessary information to do so. 
While real, the resource is instantiated and ready for use.
A **transient** resource is owned, realized and virtualized by the framegraph. 
A **retained** resource is always real and is imported into the framegraph.

**What is next?**
Asynchronous render tasks (+ resource/aliasing barriers).