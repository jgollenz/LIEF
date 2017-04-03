/* Copyright 2017 R. Thomas
 * Copyright 2017 Quarkslab
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "pyELF.hpp"

#include "LIEF/ELF/SymbolVersion.hpp"
#include "LIEF/visitors/Hash.hpp"

#include <string>
#include <sstream>

template<class T>
using getter_t = T (SymbolVersion::*)(void) const;

template<class T>
using setter_t = void (SymbolVersion::*)(T);

void init_ELF_SymbolVersion_class(py::module& m) {

  // SymbolVersion version object
  py::class_<SymbolVersion>(m, "SymbolVersion")
    .def_property("value",
        static_cast<getter_t<uint16_t>>(&SymbolVersion::value),
        static_cast<setter_t<uint16_t>>(&SymbolVersion::value),
        "- `0` : The symbol is local\n"
        "- `1` : The symbol is global\n\n"
        "All other values are used for versions in the own object or in any of\n"
        "the dependencies.  This is the version the symbol is tight to.")

    .def_property_readonly("has_auxiliary_version",
        &SymbolVersion::has_auxiliary_version,
        "Check if this symbols has a " RST_CLASS_REF(lief.ELF.SymbolVersionAux) "")

    .def_property_readonly(
        "symbol_version_auxiliary",
        static_cast<SymbolVersionAux& (SymbolVersion::*)(void)>(
          &SymbolVersion::symbol_version_auxiliary),
        "Return the " RST_CLASS_REF(lief.ELF.SymbolVersionAux) " associated with this version",
        py::return_value_policy::reference_internal)


    .def("__eq__", &SymbolVersion::operator==)
    .def("__ne__", &SymbolVersion::operator!=)
    .def("__hash__",
        [] (const SymbolVersion& sv) {
          return LIEF::Hash::hash(sv);
        })

    .def("__str__",
        [] (const SymbolVersion& symbolVersion)
        {
          std::ostringstream stream;
          stream << symbolVersion;
          std::string str =  stream.str();
          return str;
        });
}