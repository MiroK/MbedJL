// This file is a part of Julia. License is MIT: https://julialang.org/license

#include <julia.h>
#include <stdio.h>
#include <time.h>
#include <Python.h>

int jlfunc(int n, double *a, double *b){
    jl_init();
    // define julia function and call it
    jl_eval_string("my_func(x) = sin(x)");
    jl_function_t *func = jl_get_function(jl_current_module, "my_func");

    for (int i=0; i<n; i++) {
        jl_value_t* arg = jl_box_float64(a[i]);
        double ret = jl_unbox_float64(jl_call1(func, arg));
        b[i] = ret;
    }

    int ret = 0;
    jl_atexit_hook(ret);
    return ret;
}

int cfunc(int n, double *a, double *b){
    for (int i=0; i<n; i++) {
        b[i] = sin(a[i]);
    }
    return 0;
}

int pyfunc(int n, double *a, double *b){
  double *py_a, *py_b; 
  Py_Initialize(); 
  PyObject *pName, *pModule, *pFunc, *pArgs, *pValue; 
  pName = PyString_FromString("test");
  pModule = PyImport_Import(pName);
  if (pModule != NULL) {
    pFunc = PyObject_GetAttrString(pModule, "test");
    if (pFunc && PyCallable_Check(pFunc)) {
      for (int i=0; i<n; i++) {
        py_a = &a[i]; 
       
//        std::cout <<" a value "<<*py_a<<std::endl; 
        pArgs = Py_BuildValue("d",a[i]); 
//        std::cout <<" a build "<<a[i] << std::endl; 
        pValue = PyObject_CallFunction(pFunc, "d", a[i]);
//        std::cout <<" a call "<<std::endl; 
        b[i] = PyFloat_AsDouble(pValue); 
 //       std::cout <<" a parse "<<b[i] <<std::endl; 
      } 
      } else {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", "test2");
        return 1;
      }
    } else {
      PyErr_Print();
      fprintf(stderr, "Failed to load \"%s\"\n", "test");
      return 1;
    }
    Py_Finalize(); 
}

int main()
{
    int n = 10000000; 
    double *a = (double*)malloc(n*sizeof(double));
    double *b = (double*)malloc(n*sizeof(double));

    for (int i=0; i<n; i++) {
      a[i] = i / (1.0*n); 
    }

    int c0 = clock();  
    cfunc(n, a, b); 
    double cc = (clock() - c0) * 1.0 / CLOCKS_PER_SEC;  
    printf("C time %g (%g x of C speed)\n", cc, cc/cc);

    c0 = clock();  
    jlfunc(n, a, b); 
    double c1 = (clock() - c0) * 1.0 / CLOCKS_PER_SEC;  
    printf("JL time %g (%g x of C speed)\n", c1, cc/c1);

    c0 = clock();  
    jlfunc(n, a, b); 
    c1 = (clock() - c0) * 1.0 / CLOCKS_PER_SEC;
    printf("JL time (no JIT) %g (%g x of C speed)\n", c1, cc/c1);

    c0 = clock();  
    pyfunc(n, a, b); 
    c1 = (clock() - c0) * 1.0 / CLOCKS_PER_SEC;
    printf("PY time %g (%g x of C speed)\n", c1, cc/c1);

    free(a);
    free(b);
}
