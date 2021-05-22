# template-for-PS
a template I shall use for PS competitions (when I am allowed to do so)


* Aim: leaves less space for memory-related mistakes
* Aim: easy to use (once you know what data structures the classes implement)
* Aim: flexible
* Aim: Practice c++17 features

## TODO

* Support for operations on Z_p
* Implement some boilerplates (like findAll(std::string from, std::string to) )
* Make a bunch of segtrees
  * Vanilla
  * With lazyprop
  * Persistent
* Make a `bigint` class
* Make a `bigbigint` class
* Make some templates for using STL (priority_queue)
* A better documentation format...?

# Notes

## stetsegtree.hpp

This is a header-only library that implements an arbitrary-type Dynamic/LazyProp/Persistent Segment tree. 

This class implements an arbitrary-type Dynamic/LazyProp/Persistent Segment tree.
Interface to users is provided through several factory functions - **users may choose to use the type of segtree they wish to, but it is always a variation of this class that is provided.** This design was chosen to maximize maintainability.(Else, I would be writing the same class eight times.)

## factory functions

**guideline: All instantiations of `class StetSegTree` should happen through these factory functions.** (If you would like to *mentally torture yourself* by disobeying this guideline, you are (of course) welcome to do so)
- `newSegTree(T&& initValues, size_t sz, Compare reducer)`
- `newSegTree(T&& initValues, size_t sz, Compare reducer, T defaultValue)`
- `newLazyPropSegTree(T&& initValues, size_t sz, Compare reducer)`
- `newLazyPropSegTree(T&& initValues, size_t sz, Compare reducer, T defaultValue)`
- `newPersistentSegTree(T&& initValues, size_t sz, Compare reducer, T defaultValue, CheckpointType initCheckpoint=(size_t)0)`
- `newLazyPropPersistentSegTree(T&& initValues, size_t sz, Compare reducer, T defaultValue, CheckpointType initCheckpoint=(size_t)0)`

the `defaultValue` argument is optional for the first two - if this argument is present, the factory function yields a dynamic segment tree. (do note that whether or not a SegTree is dynamic should have no effect on its behavior; perhaps except for its memory allocation behavior.)

### `class StetSegTree`

**TEMPLATE ARGUMENTS** listed for sake of explanation
- typename `T` : elements' type
- typename `Compare` : type of `reducer`; Callable, takes two arguments related to T, returns T 
- typename `CheckpointType` : type of `checkpoint` names.
- typename `Lazy`: if this is `LazyType` we enable some set of functions
- typename `PersistentType`: if type is `PersistentType` we enable some set of functions/classes

**CLASS INVARIANTS**
- When observed, `node.val == reducer(node.left->val,node.right->val)`
- When observed, `node.val` should have the value it should have.

**TYPEDEFS and CONSTANTS**
- `using Interval = std::pair<size_t,size_t>`
- `const T defaultVal`: the `val` value to assume for nonassigned terminal nodes
- `const size_t sz` : max. number of elements that can be managed by the tree

**`class ref`** supports
- cast to type `T` (for printing, etc)
- assignment to a single element (s.t. StetSegTree class invariant is preserved)
- maybe other operators, if T has any other

**`class TreeView`**
- a read-only view into the tree.

**`StetSegTree::ref operator[](size_t)`**
- example usage: `tree[i] = 3` (this will preserve class invariant and make life easier)

**`T reduce(Interval)`**
- example usage: `tree.reduce({3,9})`
- if Interval goes out of bounds, std::out_of_range is thrown

**`void updateInterval(Interval, *a callable* )`**
- enabled iff
- example usage: `tree.updateInterval({1,5},[](Interval iv,T val){return val>3?val:3;});`
- the callable takes: an `Interval`, a `T`, and returns another `T`
  - implements group update, as in lazy propagation
  - look at the interval a node is in charge of, the node's current `val`, and then determine the new `val` for the node.
  - such feat should be possible: if not lazyprop is impossible to do efficiently anyways
- how to check if this is a callable that does that?

**`void makeCheckpoint(CheckpointType checkpointName)`**
- enabled iff `` 
- Archives current tree
- saves state by the "name" `checkpointName`
- saved state is reaccessible by this name
- another `makeCheckpoint` call with the same name will cause states to be overwritten

**`TreeView loadCheckpoint(CheckpointType checkpointName)`**
- "loads" Tree state
- `TreeView` only has one method: `reduce` - which is quite the same as above.
