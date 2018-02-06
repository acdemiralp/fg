Definitions:

- What is a framegraph?
  - A directed acyclic graph consisting of render tasks and resources.

- What is a render task?
  - An encapsulation of a compute or graphics task to be performed on the GPU.
  - Consists of a name, a setup function, an execute function and references to the data it operates upon.

- What is a resource?
  - An encapsulation of data created, modified or used by a render task.
  - What is a virtual resource?
    - A resource which has not yet been instantiated but contains all information necessary to do so.
    - Consists of an ID, a name (optional), a type and a description.
  - What is a real resource?
    - A resource which has been instantiated (either from its virtual representation or through external means).
    - Example: An OpenGL buffer.
  - What is a transient resource?
  	- A resource created, realized, managed and unrealized by the framegraph.
  - What is a retained resource?
  	- An always real resource imported into the framegraph (although a declaration may still be necessary as metadata during setup).

- How does it work?
  - The user instantiates a framegraph.
  - Adds render tasks to the framegraph.
    - The setup function of the render task is inlined to this step and sets up its virtual dependencies and dependents.
  - Calls framegraph.compile (), which removes unused render tasks / resources and computes the lifetime of each resource.
  - Calls framegraph.traverse(), which iterates through each render task, realizes its necessary resources and then calls its execute().

- Notes
  - Framegraph is a heterogenous graph since it consists of two independent vertex types; render tasks and resources. Moreover, a render task can only have an edge to a resource and a resource can only have an edge to a render task, a rule which does not have a name. Yet.
