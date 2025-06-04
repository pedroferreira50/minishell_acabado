/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlos- <scarlos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:31:59 by scarlos-          #+#    #+#             */
/*   Updated: 2025/06/04 12:16:24 by scarlos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <limits.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <string.h>
# include <stdbool.h>
# include <stdio.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"

extern int	g_signal;

typedef struct s_var
{
	char			*name;
	char			*value;
	int				exported;
	struct s_var	*next;
}	t_var;

typedef struct s_parse_result
{
	char	**args;
	char	*quote_types;
}	t_parse_result;

typedef struct s_redirection
{
	char	*file;
	int		append;
}	t_redirection;

typedef struct s_command_data
{
	char			**commands;
	char			***arguments;
	char			*heredoc_delim;
	char			**input_files;
	t_redirection	**out_redirs;
	int				*num_out_redirs;
	int				num_commands;
	int				num_pipes;
	int				heredoc_quoted;
}	t_command_data;

typedef struct s_shell
{
	char	**envp;
	t_var	*vars;
	int		exit_status;
	int		is_counting;
	bool	is_save_to_execute;
}	t_shell;

typedef struct s_parse
{
	const char	*cmd;
	char		**args;
	char		quote_char;
	char		*quote_types;
	int			args_count;
	int			in_quotes;
	int			brace_count;
	size_t		i;
	size_t		start;
}	t_parse;

typedef struct s_exec_state
{
	int	pipefd[2];
	int	prev_pipe_read;
	int	heredoc_fd;
	int	i;
}	t_exec_state;

typedef struct s_indices
{
	size_t	i;
	size_t	j;
}	t_indices;

typedef struct s_parse_state
{
	t_indices	idx;
	int			command_index;
}	t_parse_state;

typedef struct s_count_context
{
	char			**args;
	int				count;
	t_command_data	*data;
	int				*arg_counts;
	t_shell			*shell;
}	t_count_context;

typedef struct s_quote_segment
{
	char	*content;
	char	quote_type;
	int		start;
	int		end;
}	t_quote_segment;

typedef struct s_redirect_context
{
	char			**args;
	t_command_data	*data;
	t_indices		*indices;
	t_shell			*shell;
	int				cmd_index;
}	t_redirect_context;

typedef struct s_redirect_params
{
	char			**args;
	t_command_data	*data;
	t_indices		*indices;
	int				cmd_index;
}	t_redirect_params;

typedef struct s_output_params
{
	t_redirect_params	base;
	const char			*token;
}	t_output_params;

// main.c
t_shell			*get_shell(void);
void			init_shell(t_shell *shell, char **envp);
char			*read_input(void);
int				process_input(char *input, t_shell *shell);
void			finalize_shell(t_shell *shell);
void			update_env_var(char *name, char *value, t_shell *shell);
void			handle_command(char *input, t_shell *shell);
void			handle_assignment_non_export(char *input,
					t_parse_result *parsed);

// main_utils.c
void			cleanup_parse_data(t_parse_result *parsed, char **expanded_args,
					char **filtered_args);

// handle_command_utils.c
void			cleanup_command_data(t_command_data *data);
char			**parse_and_expand_command(char *input, t_shell *shell,
					t_parse_result *parsed);
char			**create_filtered_args(char **expanded_args);
t_command_data	*prepare_command_data(char **filtered_args, t_shell *shell);
void			execute_command_pipeline(t_command_data *data, t_shell *shell);
// signal.c
void			set_signals_interactive(void);
void			set_signals_noninteractive(void);
void			ignore_sigquit(void);

//handle_operator.c

void			handle_pipe(t_command_data *data, int *command_index,
					t_shell *shell);
// handle_redirect functions
int				validate_redirect_syntax(char **args, t_indices *indices,
					t_shell *shell);
int				handle_input_redirect(t_redirect_params *params,
					t_shell *shell);
int				open_output_file(char **args, t_indices *indices,
					const char *token, t_shell *shell);
int				add_output_redirection(t_output_params *params);
int				handle_output_redirect(t_output_params *params, t_shell *shell);
void			handle_redirect(t_redirect_context *ctx);
int				set_redirection_data(t_output_params *params);
int				process_input_file(t_redirect_params *params, t_shell *shell);
int				update_input_files(t_redirect_params *params, t_shell *shell);

// handle_heredoc functions
char			*process_quoted_delimiter(char *delim, size_t len);
char			*clean_mixed_quotes(char *delim, size_t len);
void			set_heredoc_delimiter(t_command_data *data, char *delim);
int				validate_heredoc_args(char **args, t_indices *indices,
					t_shell *shell);
void			handle_heredoc(char **args, t_command_data *data,
					t_indices *indices, t_shell *shell);

// env_utils.c
char			**copy_envp(char **envp);

// parsing_utils.c
int				count_args(char **args);
void			add_argument(t_parse *state);

// string_utils.c
char			*ft_strndup(const char *src, size_t n);

//error.c
void			error_operator(char op, t_shell *shell);
// Enhanced error handling utility functions
void			print_error_simple(const char *message, int exit_code,
					t_shell *shell);
void			print_error_command(const char *command, const char *message,
					int exit_code, t_shell *shell);
void			print_error_token(const char *token, int exit_code,
					t_shell *shell);

void			initialize_state(t_parse *state, const char *cmd);
int				check_errors(t_parse *state, t_shell *shell,
					int last_was_operator);
void			finalize_result(t_parse *state, t_parse_result *result,
					const char *cmd);
// process_token.c
void			process_token(t_parse *state, int *last_was_operator);
//parse_input_utils.c
int				is_operator(const char *token);
int				*initialize_arg_counts(int count, t_shell *shell);
void			finalize_arguments(t_command_data *data, int *arg_counts);
//parse_input.c
void			parse_input(char **args, int count, t_command_data *data,
					t_shell *shell);
void			populate_command(char **args, int *arg_counts,
					t_command_data *data, t_parse_state *state);
void			populate_argument(char **args, t_command_data *data,
					t_parse_state *state);
void			populate_commands(char **args, int *arg_counts,
					t_command_data *data, t_shell *shell);

//parse_input/count_commands.c
void			count_commands(char **args, int count, t_command_data *data,
					int *arg_counts);

//parse_input/count_commands_utils.c
void			handle_pipe_validation(t_count_context *ctx, t_indices *idx);
void			process_heredoc_operator(t_count_context *ctx, t_indices *idx);
void			process_redirect_operator(t_count_context *ctx, t_indices *idx,
					int command_index);
int				validate_args_and_init(t_count_context *ctx, t_indices *idx,
					int *command_index);
int				check_current_arg(t_count_context *ctx, size_t i);

//builtins
// check_execute_builtins.c
int				child_builtin(int *i, t_shell *shell, t_command_data *data);
int				parent_builtin(t_command_data *data, t_exec_state *state,
					t_shell *shell);

// builtins_utils.c
int				ft_isspace(int c);
int				check_builtin(char *command);
int				execute_builtin_command(char *command, char **args,
					t_shell *shell, int *i);
int				handle_input_redirection(t_command_data *data, int *i,
					int original_stdin, t_shell *shell);
int				handle_output_redirection(t_command_data *data, int *i,
					int original_stdout, t_shell *shell);
void			restore_fds(int original_stdin, int original_stdout);
//cd
int				ft_cd(char **args, int *i, t_shell *shell);
//cd_utils
char			*get_env_value(char **envp, const char *name);
int				determine_cd_path(char **args, char **path, t_shell *shell);
int				update_pwd_vars(char *old_pwd, t_shell *shell);
int				gotohome(char **path, t_shell *shell);
int				goto_oldpwd(char **path, t_shell *shell);
//echo
void			ft_echo(char **args, t_shell *shell);
//env
void			ft_env(char **envp, t_shell *shell);
//export
int				ft_export(char **args, t_shell *shell);
void			export_var(char *name, char *value, t_shell *shell);
int				print_exported_env(t_shell *shell);
int				find_env_var_index(t_shell *shell, const char *name);
char			**copy_envp_with_update(t_shell *shell, char *new_entry,
					int replace_index);
void			add_env_var(t_shell *shell, char *new_entry, char *name);
char			*create_env_entry(char *name, char *value);
void			insertion_sort_env(char **env);
int				handle_invalid_identifier(char *name, t_shell *shell);
int				handle_builtin_invalid_option(char *arg, char *cmd_name,
					char *usage);
//pwd
int				ft_pwd(void);
//unset
int				ft_unset(char **args, t_var **vars, char ***envp);
//exit
void			ft_exit(char **args, t_shell *shell);
//loc_vars
int				is_valid_var_name(const char *name);
void			store_var(char *name, char *value, t_var **vars);

int				is_var_assignment(const char *input);
void			handle_var_assignment(const char *input, t_shell *shell);

//expand_vars folder
// calc_size.c
size_t			calc_expanded_size(const char *arg, char quote_type,
					t_shell *shell);
// fill_expand.c
void			fill_expanded(char *dest, const char *src, char quote_type,
					t_shell *shell);
// fill_expanded_utils.c
void			fill_quotes(char *dest, const char *src, t_indices *indices,
					char quote_type);
void			fill_exit_status(char *dest, t_indices *indices,
					t_shell *shell);
void			fill_var_name(char *dest, const char *src, t_indices *indices,
					t_shell *shell);
// var_expansion.c
size_t			calc_var_size(const char *arg, t_indices *indices,
					t_shell *shell);
// var_expansion.c
size_t			handle_exit_status(char *dest, int fill, size_t *i,
					t_shell *shell);
size_t			get_var_len(const char *str, size_t i, t_var *vars,
					char **envp);
char			*get_var_value_helper(const char *name, t_var *vars,
					char **envp);
char			*expand_variables(const char *arg, char quote_type,
					t_shell *shell);
char			*expand_mixed_quotes(const char *str, t_shell *shell);

// parse_quote_segments.c
void			skip_quoted_section(const char *str, int *i);
void			skip_unquoted_section(const char *str, int *i);
int				count_segments(const char *str);
int				find_quote_end(const char *str, int start, char quote_char);
t_quote_segment	*parse_quote_segments(const char *str, int *count);

//find_command_path.c
char			*find_command_path(char *command, t_shell *shell);

//pipes_and_execution folder
// pipeline.c
void			finalize_execution(t_exec_state *state, pid_t *pids,
					t_command_data *data, t_shell *shell);
void			setup_pipes_and_redirections(t_command_data *data,
					t_exec_state *state, int num_commands, t_shell *shell);
int				setup_pipeline(t_command_data *data, t_exec_state *state,
					t_shell *shell);
// heredoc.c
int				setup_heredoc(const char *delimiter, t_shell *shell,
					int suppress_expansion);

//  execute_commands.c
void			run_pipeline(t_command_data *data, t_exec_state *state,
					t_shell *shell, pid_t *pids);
void			fork_child(t_command_data *data, t_exec_state *state,
					t_shell *shell, pid_t *pids);
void			execute_command(int *i, t_shell *shell, pid_t *pids,
					t_command_data *data);
void			handle_interrupt_signals(pid_t *pids, t_exec_state *state,
					t_command_data *data);
void			handle_wait_status(int status, t_shell *shell);
void			execute_commands(t_command_data *data, t_shell *shell);
void			wait_commands(pid_t *pids, t_command_data *data,
					t_shell *shell);
//expand_vars/command_processor.c
void			expand_and_validate(char **tokens, char *quote_types,
					t_shell *shell);
t_parse_result	parse_command(const char *cmd, t_shell *shell);

char			**expand_tokens(char **tokens, char *quote_types,
					t_shell *shell);
char			*expand_tilde(char *token, t_shell *shell);
int				validate_command(char **args, t_shell *shell);
int				has_mixed_quotes(const char *str);

//remove_quotes.c
char			*remove_quotes(char *str);

//free
void			free_data_commands(char **commands, int num_commands);
void			free_data_arguments(char ***arguments, int num_commands);
void			free_all_vars(t_var **vars);
void			free_args(char **args, t_command_data *data);
void			free_command_data(t_command_data *data);
void			free_state(t_parse *state);

void			print_arguments(t_command_data *data);
void			print_args(char **args);
#endif
