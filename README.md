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

This is a header-only library that implements an arbitrary-type LazyProp/Persistent Segment tree. 

Interface to users is provided through several factory functions - **users may choose to use the type of segtree they wish to, but it is always a slight variation of the same class that is provided.** This design was chosen to maximize maintainability.(Else, I would be writing the same class eight times.)

## factory functions

**guideline: All instantiations of `class StetSegTree` should happen through these factory functions.** (If you would like to *mentally torture yourself* by disobeying this guideline, you are (of course) welcome to do so)
- `newSegTree(T&& initValues, size_t sz, Reducer reducer)`
- `newLazyPropSegTree(T&& initValues, size_t sz, Reducer reducer)`
- `newPersistentSegTree(T&& initValues, size_t sz, Reducer reducer, T defaultValue, CheckpointType initCheckpoint=(size_t)0)`
- `newLazyPropPersistentSegTree(T&& initValues, size_t sz, Reducer reducer, T defaultValue, CheckpointType initCheckpoint=(size_t)0)`

### `class StetSegTree`

**TEMPLATE ARGUMENTS** listed for sake of explanation
- typename `T` : elements' type
- typename `ReducerType` : type of `reducer`; Callable, takes two arguments related to T, returns T 
- typename `OperationIDType` : Did you know that the space of maps we consider when implementing LazyProp is approximately isomorphic to an infinite abelian group? This Type should be the element of that group. Defaults to `size_t`. (in other words, `\mathbb{Z}`.
- `std::function<T(const Interval& const T& origVal, const OperationIDType&)> operation` : Users must dictate how operations on intervals behave, given the interval size, original value on node, and the `OperationID`. (See examples below for detail)
- `std::function<OperationIDType(const OperationIDType&, const OperationIDType&)>` : Users must dictate a policy on how operations on intervals are concatenated.
- typename `CheckpointType` : type of `checkpoint` names. Defaults to `size_t`


**CLASS INVARIANTS**
- "Observation" := access to `node.val`.
- When observed, `node.val == reducer(node.left->val,node.right->val)`
- When observed, `node.val` should have the value it should have. i.e. all operations should have been applied to the observed value in the order they were enqueued.
- When `node.val` must be modified but `node.version` is different to its parent's, a new node shall be made.

**TYPEDEFS and CONSTANTS**
- `using Interval = std::pair<size_t,size_t>`
- `const T defaultVal`: the `val` value to assume for nonassigned terminal nodes
- `const size_t sz` : max. number of elements that can be managed by the tree

**`class ref`** supports
- cast to type `T` (for printing, etc)
- assignment to a single element (s.t. StetSegTree class invariant is preserved)
- maybe other operators, if T has any other

**`StetSegTree::ref operator[](size_t)`**
- example usage: `tree[i] = 3` (this will preserve class invariant and make life easier)

**`T reduce(Interval, CheckpointType)`**
- example usage: `tree.reduce({3,9})`
- if Interval goes out of bounds, std::out_of_range is thrown

**`void updateInterval(Interval, OperationIDType)`**
- enabled iff
- example usage: `tree.updateInterval({1,5}, -3 );

**`void makeCheckpoint(CheckpointType checkpointName)`**
- Archives current tree
- saves state by the "name" `checkpointName`
- saved state is reaccessible by this name
- another `makeCheckpoint` call with the same name will cause states to be overwritten
  - not recommended, as doing so will lead to a memory leak.

