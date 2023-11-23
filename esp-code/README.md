
# LIMPID convention for C coding

<p align="center">
  <img src="esp-code/images/diagram_system.png" width="300">
</p>

## Description

This naming is dedicated to developing embedded C code specifically for the ESP-32 microcontroller. To maintain consistency and enhance code readability, we have established the following naming conventions.
We follow a consistent naming convention for various elements to ensure clarity and maintainability.

The prefix should always indicate the driver parameter and the convention should follow  snake_case.

### Drivers 

The drivers in the LIMPID project should always have the prefix "LMPD" before each name. 

```c
LMPD_ph.c
LMPD_blt.c
```

### Common variables

- Use meaningful names that describe the purpose of the variable.

```c
int ph_result;
float tb_sensitivity;
char blt_code[50];
```

### Constant variables

- Use uppercase letters with underscores to separate words.
- Provide a clear and concise name that indicates the purpose of the constant.

```c
#define PH_LEVEL_SIZE 100
```

### Global variables

- Prefix global variables with "g_" to distinguish them from local variables.

```c
int g_blt_address;
```
### Pointer variables

- Include the type in the variable name to clarify that it's a pointer.

```c
int* p_ph_integer;
char* p_ph_buffer;
```

### Function convention

- Use meaningful names that describe the action or purpose of the function.

```c
void ph_function1(); //potential hydrogen
void do_function2(); //dissolved oxygen
void tb_function3(); //turbidity 
```


