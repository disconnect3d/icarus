types
======


future state
===============

This document is a work in progress and reflects the future state of Icarus


high level
----------

In Icarus the concept of a type has a few moving parts

* ic_type - the concept of a type, either builtin or user defined, and points to ic_type_decl or ic_type_builtin
* ic_type_builtin - the information needed for a builtin
* ic_type_decl - the information captured for a user declared type
* ic_type_ref - a reference to a type, built during parse and modified during analysis


eventually we want the following dep. diagram:

    ic_type_builin - a builtin type
        ^
        |      ic_type_decl - a user defined type
        |            ^
        \            |
         \          /
          \        /
           \      /
            \    /
             \  /
              \/
            ic_type - a type, one instance per type (Int, String, Foo, Bar, ...)
               ^
               |
               |
               |
           ic_type_ref - a use of a type, say the type of a variable


ic_type_ref
-----------

ic_type_ref(s) are constructed during parse time and are a use of a type
at parse time most type references start their life as either 'unknown' (type inference)
or 'string' (declared as string in source, say `name::String`)

In icarus a type_ref has 4 possible states:

* unknown - we have no information about this type, rely on type inference
* string - set by parser from source text of program
* reference - a reference to a known ic_type - this is the most complete a type_ref can be
* error - a user fixable error occurred during inference

at the end of the analyse phase it is an error if any ic_type_ref is not in the `reference` state
as it means we could not resolve the reference to a concrete type.


ic_type
-------

ic_type is an abstraction over both builtin and user defined types

an ic_type thus has 2 states:

* builtin - this type is a builtin type defined in the icarus source code
* user - this type is a type defined by the user in their program source




current state / old documentation
===============

THE FOLLOWING DOCUMENTATION IS DEPRECATED AND WILL BE SLOWLY BE MADE MORE AND MORE INCORRECT


type_ref
--------

In icarus a type_ref has 5 possible states:

* unknown - we have no information yet about this type
* string - set by parser - we have a string type name from the parser
* tdecl - set by inference - we have a type_decl found from the string or context
* builtin - set by inference - we have a type_builtin froun from the string or context
* error - a user fixable error occurred during inference

This models a progression from least to most known in the case of

    unknown -> string -> tdecl
    unknown -> string -> builtin
    string -> tdecl
    string -> builtin

After parsing all types will either be `unknown` or `string`

After analysis all types will either be `builtin`, `tdecl` or `error`

we now have a type for this concept `ic_type_ref`

most of the code using types exists within `src/parse/data/*` and
currently all of this code uses an `ic_symbol`

This should be used in:

* fields - on tdecls and fdecls
* variables
* return values - fdecls


