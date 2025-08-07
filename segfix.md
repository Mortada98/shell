# Segmentation Fault Fixes for Minishell

## Issue Description
The minishell program was experiencing segmentation faults when executing certain command sequences, particularly when running `""`, `$a`, `ls`, and `exit` commands in sequence.

## Root Cause Analysis
Using GDB, the segmentation fault was traced to:
- **Location**: `src/main_utils.c:20` in the `is_number()` function
- **Cause**: NULL pointer dereference when attempting to access `str[i]` where `str` was NULL
- **Call Stack**: `is_number()` ← `my_exit()` ← `excute_redirection_of_parent()` ← execution chain

The issue occurred because the exit command validation was calling `is_number((*cmd)->args[1])` without checking if `(*cmd)->args[1]` was NULL.

## Fixes Applied

### 1. Fixed NULL Pointer Check in Exit Command (`src/exit_command.c`)

**Problem**: The code was calling `is_number()` on a potentially NULL pointer.

**Before**:
```c
if ((*cmd)->args[2] && is_number((*cmd)->args[1]))
{
    write(2, "minishell: exit: too many arguments\n", 36);
    set_status(1);
    *error = 0;
    return ;
}
```

**After**:
```c
if ((*cmd)->args[2] && (*cmd)->args[1] && is_number((*cmd)->args[1]))
{
    write(2, "minishell: exit: too many arguments\n", 36);
    set_status(1);
    *error = 0;
    return ;
}
```

### 2. Added Defensive Programming in `is_number()` Function (`src/main_utils.c`)

**Problem**: Function didn't handle NULL input gracefully.

**Before**:
```c
int	is_number(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '\0')
		return (0);
	// ... rest of function
}
```

**After**:
```c
int	is_number(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	if (str[i] == '\0')
		return (0);
	// ... rest of function
}
```

### 3. Fixed Garbage Collector NULL Checks (`src/clean_up.c`)

**Problem**: Missing check for `*g_gc` being NULL.

**Before**:
```c
void	gc_cleanup(void)
{
	t_gc_node	*current;
	t_gc_node	*next;
	t_gc		**g_gc;

	g_gc = gc_get();
	if (!*g_gc)
		return ;
	// ... rest of function
}
```

**After**:
```c
void	gc_cleanup(void)
{
	t_gc_node	*current;
	t_gc_node	*next;
	t_gc		**g_gc;

	g_gc = gc_get();
	if (!g_gc || !*g_gc)
		return ;
	// ... rest of function
}
```

Similar fix applied to `gc_cleanup_partial()`.

### 4. Fixed Environment Cleanup Check (`src/main.c`)

**Problem**: Checking `env[0]` instead of `env` for NULL.

**Before**:
```c
void	clean_up(char **env)
{
	gc_cleanup();
	if (env[0])
	{
		free_2d_array(env);
	}
	rl_clear_history();
}
```

**After**:
```c
void	clean_up(char **env)
{
	gc_cleanup();
	if (env)
	{
		free_2d_array(env);
	}
	rl_clear_history();
}
```

### 5. Standardized Memory Management (`src/dup_parent_and_child.c`)

**Problem**: Manual environment freeing instead of using consistent `free_2d_array()` function.

**Before**:
```c
int i = 0;
while((*env)[i])
{
	free((*env)[i]);
	i++;
}
free(*env);
```

**After**:
```c
free_2d_array(*env);
```

### 6. Fixed Double History Addition (`src/main.c`)

**Problem**: `add_history()` was called twice for each command.

**Before**:
```c
refer = process_line(&token, &line, *env, &data);
if (refer == 0)
	continue ;
if (refer == 2)
	break ;
add_history(line);  // <-- Duplicate call
if (!parsing_process(&token, *env, data, &cmd))
	continue ;
```

**After**:
```c
refer = process_line(&token, &line, *env, &data);
if (refer == 0)
	continue ;
if (refer == 2)
	break ;
if (!parsing_process(&token, *env, data, &cmd))
	continue ;
```

(The `add_history()` call was kept in `process_line()` function)

### 7. Fixed Pointer Syntax (`src/main_helper.c`)

**Problem**: Incorrect pointer dereferencing syntax.

**Before**:
```c
if (*line[0] == '\0')
	return (0);
```

**After**:
```c
if ((*line)[0] == '\0')
	return (0);
```

## Testing Results

After applying these fixes:
- ✅ The original failing sequence (`""`, `$a`, `ls`, `exit`) now works without segfault
- ✅ Simple exit commands work correctly
- ✅ Variable expansion works properly
- ✅ Memory management is more robust
- ✅ All basic shell functionality remains intact

## Prevention Measures

To prevent similar issues in the future:
1. Always check for NULL pointers before dereferencing
2. Use consistent memory management functions
3. Add defensive programming checks in utility functions
4. Test edge cases with empty/NULL inputs
5. Use debugging tools (GDB, Valgrind) for thorough testing

## Files Modified

- `src/exit_command.c` - Added NULL check before calling `is_number()`
- `src/main_utils.c` - Added NULL check in `is_number()` function
- `src/clean_up.c` - Fixed garbage collector NULL checks
- `src/main.c` - Fixed environment check and removed duplicate history call
- `src/dup_parent_and_child.c` - Standardized memory management
- `src/main_helper.c` - Fixed pointer dereferencing syntax
