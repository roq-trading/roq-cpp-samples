This library selects the default compiler standard version (at least c++14)
and will otherwise hide types that are incompatible with dependencies.

In particular, certain abseil-cpp functions exports a signature depending on
the **default** compiler standard version being used to compile this project.
When compiled with c++14, abseil-cpp will **not** support `std::string_view`
even if the public interface allows this type.
