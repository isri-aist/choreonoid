/*!
  @author Shin'ichiro Nakaoka
*/

#include "PyBase.h"

namespace py = pybind11;
using namespace cnoid;

namespace {

void checkIfItemSubclass(PyObject* typeObject)
{
    auto &types = py::detail::get_internals().registered_types_cpp;
    auto it = types.find(std::type_index(typeid(Item)));
    pybind11::detail::type_info* itemClass = 0;
    if (it != types.end())
        itemClass = (pybind11::detail::type_info *) it->second;

    int isSubclass = PyObject_IsSubclass(typeObject, (PyObject*)itemClass->type);
    if(isSubclass <= 0){
        PyErr_SetString(PyExc_TypeError, "argument for specifying the item list type must be an Item class");
        throw py::error_already_set();
    }
}


} // namespace

namespace cnoid {
py::list getPyNarrowedItemList(const ItemList<>& orgItemList, py::object itemClass)
{
    checkIfItemSubclass(itemClass.ptr());

    py::list narrowedItemList;
    for(int i=0; i < orgItemList.size(); ++i){
        py::object item(py::cast(orgItemList[i]));
        if(PyObject_IsInstance(item.ptr(), itemClass.ptr()) > 0){
            narrowedItemList.append(item);
        }
    }
    return narrowedItemList;
}

py::list getPyNarrowedItemList(py::list orgItemList, const PyTypeObject* itemClass)
{
    checkIfItemSubclass((PyObject*)itemClass);
    
    py::list narrowedItemList;
    const int n = py::len(orgItemList);
    for(int i=0; i < n; ++i){
        py::object item = orgItemList[i];
        if(PyObject_IsInstance(item.ptr(), (PyObject*)itemClass) > 0){
            narrowedItemList.append(item);
        }
    }
    return narrowedItemList;
}

py::object getPyNarrowedFirstItem(const ItemList<>& orgItemList, py::object itemClass)
{
    checkIfItemSubclass(itemClass.ptr());

    for(int i=0; i < orgItemList.size(); ++i){
        py::object item(py::cast(orgItemList[i]));
        if(PyObject_IsInstance(item.ptr(), itemClass.ptr()) > 0){
            return item;
        }
    }
    return py::object();
}

} // namespace cnoid