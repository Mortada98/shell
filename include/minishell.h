/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouizak <mbouizak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:22:52 by helfatih          #+#    #+#             */
/*   Updated: 2025/08/07 14:52:04 by mbouizak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# ifndef GREEN
#  define GREEN "\001\033[1;32m\002"
# endif
# ifndef BLUE
#  define BLUE "\001\033[4;34m\002"
# endif
# ifndef WHITE
#  define WHITE "\001\033[1;37m\002"
# endif
# ifndef YELLOW
#  define YELLOW "\001\033[1;33m\002"
# endif
# include "../my_libft/libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <uchar.h>
# include <unistd.h>

extern int				g_value;

// Garbage Collector structures
typedef struct s_gc_node
{
	void				*ptr;
	struct s_gc_node	*next;
}						t_gc_node;

typedef struct s_gc
{
	t_gc_node			*allocations;
	size_t				count;
}						t_gc;

typedef enum s_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HERDOC
}						t_token_type;

typedef struct s_token
{
	char				*av;
	bool				info;
	bool				found;
	t_token_type		type;
	struct s_token		*next;
	struct s_token		*prev;
}						t_token;

// typedef struct s_exec
// {
// 	int					prev_fd;
// 	int					fd[2];
// 	pid_t				pid;
// 	int					fd_out;
// 	int					fd_in;
// 	t_command			*curr;
// 	int					saved_stdin;
// 	pid_t				pids[1024];
// 	int					pid_count;
// 	int					save;
// 	int					hd_fd;
// 	char				*command;
// 	bool				has_command;
// }						t_exec;

typedef struct s_command
{
	char				**args;
	char				**file_input;
	char				*file_output;
	char				**herdoc;
	char				*herdoc_file;
	int					append;
	bool				file;
	bool redir_error; // Flag to mark redirection errors
	struct s_command	*next;
}						t_command;
typedef struct s_parse
{
	t_token				*current;
	t_command			*first_cmd;
	t_command			*current_cmd;
	int					i;
}						t_parse;

typedef struct s_cmd_var
{
	char				*path_env;
	char				**split_env;
	char				*complete_path;
	char				*first_join;
	struct stat			sb;
}						t_cmd_var;

typedef struct s_var
{
	char				*result;
	char				*string;
	char				*valeur;
	char				*status_str;
	bool				condition;
	bool				flag;
	size_t				i;
	size_t				j;
	size_t				old_size;
	size_t				new_size;
	size_t				start;
}						t_var;

typedef struct s_word_processing
{
	bool				should_join;
	char				*str;
	char				*word;
	int					flag;
	bool				found;
	t_token_type		value;
}						t_word_processing;

typedef struct s_data
{
	int					exit;
	int					end;
	int					start;
	bool				should_expand_outside;
	bool				should_expand_inside;
	int					count_herdoc;
	int					count_red_in;
	bool				ambigiouse;
	bool				cmd_found;
	int					flags;
}						t_data;


void	process_options(char **args, bool *has_n, int *idx);
void	print_args(char **args, int idx);
int						handle_redir_append_token(t_parse *var);
int						handle_pipe_token(t_parse *var, t_data **data);
int						handle_redir_in_token(t_parse *var, t_data **data);
int						handle_redir_out_token(t_parse *var);
int						handle_redir_out_token(t_parse *var);
int						handle_heredoc_token(t_parse *var);
int						handle_word(t_parse *var, t_data **data);
int						process_current_token(t_parse *var, t_data **data);
int						is_number(char *str);
void					my_handler(int sig);
int						process_line(t_token **token, char **line, char **env,
							t_data **data);
int						interupte_herdoc(t_command *cmd, t_data *data,
							char **env);
void					her_cmd_by_cmd(t_command *cmd, t_data *data, char **env,
							bool *heredoc_interrupted);
int						access_file(t_command *cmd);
void					handle_core_dumped(int *pids, int pid_count,
							t_data **data);
bool					empty_command(t_command *cmd);
char					*check_file(char *cmd);
void					compare_newline(char **str, bool *j, int *i);
bool					is_redirection(t_token_type type);
int						parse_redirections(t_command **current_cmd,
							t_token **current, t_data **data);
int						word_or_herdoc(t_token **current,
							t_command *current_cmd, t_data **data, int i);
int						reset_value(t_command **current_cmd, t_token **current,
							t_data **data, t_token *token);
int						another_function(t_token **current,
							t_command **current_cmd, t_data **data, int *i);
void					remplace_var(t_var *var);
void					make_the_envirement(t_var *var);
char					*split_var(size_t *i, char *str, size_t *start);
void					herdoc_condition_1(t_command **cmd, t_data **data,
							char *join);
int						herdoc_condition_2(t_command **cmd, t_data **data);
void					read_and_convert(char *buffer, int *fd,
							unsigned char *c, int *i);
void					my_server(int ig);
void					process_word(t_token **token, t_word_processing *wp);
void					create_add_token(t_token **token, char *word,
							t_token_type value, t_word_processing *wp);
char					*process_quotes(char *str, int *flag);
char					*handle_expansion(t_data *data, char *word, char **env);
void					lexe_with_space(t_token **token, int *start, int *i,
							char *word);
void					init_var2(int *start, int *i, bool *should_join);
void					check_the_last_element(t_token **token, t_data **data);
void					make_list(char *word, t_token **token);
void					join_expansion(char *str, t_token **token);
void					convert_exit_status(char **word);
size_t					count_word(char const *s, char c, char k);
int						open_output_file_0(t_command *cmd, char *filename);
int						open_output_file_1(t_command *cmd, char *filename);
void					print_open_error(char *filename);
int						check_redir_syntax(t_token **current);
int						*set_redir_error(void);
void					reset_redir_error(int value);
int						get_redir_error(void);
int						*set_redir_error(void);
void					reset_redir_error(int value);
int						get_redir_error(void);
void					join_nodes(t_token **token);
void					excute_redirection_of_parent(t_command **cmd,
							int *fd_out, t_data *data, int *fd1, char ***env);
int						is_directory_parent(t_command **cmd);
void					open_and_duplicate(t_command **cmd, int *flags,
							int *fd_out);
int						is_number(char *str);
void					my_exit_child(t_command **cmd, t_data *data,
							int *error);
void					my_exit(t_command **cmd, t_data *data, int *error);
int						make_exit(t_command *cmd);
int						validation(t_command *cmd);
void					excute_redirection_of_child(t_command **cmd,
							t_data **data, int *fd_out, int *fd_in);
int						append_or_trunc(t_command **cmd);
int						is_directory(t_command **cmd);
void					open_red_out(t_command **cmd, int *fd_out);
void					open_red_in(int *fd_in, t_command **cmd);
int						heredoc_realloc(int *i, t_command *cmd,
							t_token **current);
int						red_in_realloc(t_command *cmd, t_data **data,
							t_token **current);
char					*make_content(char *line, t_data **data);
void					mix(bool *should_join, char *line, int i);
int						*init_status(void);
void					set_status(int val);
int						get_status(void);
void					my_handler(int sig);
void					cleanup_exit_handler(int sig);
void					free_array(char **arr);
void					my_echo(t_command *cmd);
void					excute_redirection_of_child_builtin(t_command **cmd,
							int *fd_out, t_data *data, int *fd1, int *fd2,
							char ***env);
void					check_exit_status(t_command *cmd, t_data **data);
void					excute_herdoc_for_child(t_command **cmd, t_data **data,
							char **env);
bool					built_in(char *cmd);
void					execute_builtin_command(t_command *cmd, char ***env);
void					free_2d_array(char **str);
char					*get_command(char *cmd, char **env);
void					execute_command(t_command *cmd, char ***env,
							t_data **data);
void					handle_dollar(t_token **token, char *line,
							t_data **data, char **env);
void					handle_special_quot(t_token **token, char *line,
							t_data **data, char **env);
void					handle_white_spaces(t_token **token, char *line,
							t_data **data, char **env);
bool					check_somthing(char *word);
void					handle_some_cases(t_token **token, char *line,
							t_data **data, char **env);
char					*manual_realloc(char *old, size_t len);
int						handle_pipe(t_token **current, t_command **current_cmd,
							t_command *first_cmd, t_data **data);
int						handle_redir_in(t_token **current, t_command *cmd,
							t_data **data);
int						handle_redir_out(t_token **current, t_command *cmd);
int						handle_redir_append(t_token **current, t_command *cmd);
int						handle_heredoc(t_token **current, t_command *cmd,
							int *i);
bool					con(char *str);
bool					flaging(char *str);
void					make_like_bash(char *result, char *valeur, size_t *j);
int						init_var(char *str, t_var *var);
void					print_commands(t_command *cmd);
void					print_token(t_token *token);
char					*prompt(char **env);
void					make_prompt(char ***env);
t_token					*creat_token(char *line, t_token_type type,
							bool should_join, bool found);
void					handle_quote(bool *in_quot, char *quot_char, int *i,
							char *line);
void					add_token(t_token **token, t_token *new_token);
bool					is_closed_quotes(char *str);
void					handle_word_token(t_token **token, char *line,
							t_data **data, char **env);
bool					logic_of_meta(t_token *cmd, t_data **data);
t_token_type			get_token_type(char *str);
int						handle_speciale_token(t_token **token, char *line,
							int i, t_data **data);
t_token					*tokenize(char *line, t_data **data, char **env);
void					append_arg(t_command *cmd, char *str, t_data **data);
t_command				*create_command(void);
void					free_cmd(t_command *cmd);
char					*expand_env(char *str, char **env);
t_command				*parsing_command(t_token *token, t_data **data);
int						is_space(char c);
void					free_token(t_token **token);
char					*remove_quotes(char *str);
char					*get_env(char *name, char **env);
char					**copy_env(char **env);
char					*get_env(char *name, char **env);
void					cd(char *cmd, char ***env);
void					update_oldpwd(char ***env);
void					update_pwd(char ***env);
int						add_env_variable(char *new_var, char ***env);
int						my_export(char *arg, char ***env);
int						my_unset(char *name, char ***env);
void					my_pwd(void);
int						update_existing_var(char *name, char *value,
							char **env);
void					print_env(char **env);
void					print_export_env(char **env);
int						is_valid_identifier(char *name);
int						export_without_value(char *name, char ***env);

// Garbage Collector functions
t_gc					*gc_init(void);
t_gc					**gc_get(void);
void					*gc_malloc(size_t size);
char					*gc_strdup(const char *s);
void					*gc_calloc(size_t count, size_t size);
char					*gc_substr(char const *s, unsigned int start,
							size_t len);
char					*gc_strjoin(char const *s1, char const *s2);
void					gc_free(void *ptr);
void					gc_register_external(void *ptr);
void					gc_cleanup(void);
void					gc_cleanup_partial(void);
size_t					gc_get_allocation_count(void);
void					gc_print_allocations(void);

#endif
