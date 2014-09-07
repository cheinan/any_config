any_config
==========

To build, you will need to install the JSON Spirit library.
At the time of writing, JSON Spirit can be downloaded from
http://www.codeproject.com/Articles/20027/JSON-Spirit-A-C-JSON-Parser-Generator-Implemented
Once you install the library, you will need to tell the compiler
where the library headers are located.

For example, my build line is
g++ -std=c++11 -I ~/json_spirit/json_spirit_v4.07/ test_main.cpp
Modify the path after -I to the path where you installed JSON Spirit.

The code has been tested in gcc 4.8.2 and clang 3.4.

C++ Configuration Using Boost Any

This code is based on code developed at the United States
National Center for Biotechnology Information at the US
National Institutes of Health.  The original code was
authored by the US Federal Government and is in the public
domain.  The  contribution of NCBI is gratefully acknowleged.

This code uses the Loki Library by Andrei Alexandrescu whose
work is gratefully acknowledged.

The Loki Library
Copyright (c) 2001 by Andrei Alexandrescu
This code accompanies the book:
Alexandrescu, Andrei. "Modern C++ Design: Generic Programming and Design 
    Patterns Applied". Copyright (c) 2001. Addison-Wesley.
Permission to use, copy, modify, distribute and sell this software for any 
    purpose is hereby granted without fee, provided that the above copyright 
    notice appear in all copies and that both that copyright notice and this 
    permission notice appear in supporting documentation.
The author or Addison-Wesley Longman make no representations about the 
    suitability of this software for any purpose. It is provided "as is" 
    without express or implied warranty.
