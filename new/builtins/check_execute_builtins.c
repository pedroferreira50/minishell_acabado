/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_execute_builtins.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pviegas- <pviegas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 16:06:42 by scarlos-          #+#    #+#             */
/*   Updated: 2025/06/05 03:48:55 by pviegas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


int	execute_builtin_command(char *command, char **args, t_shell *shell, int *i)
{
	int	result;

	if (ft_strcmp(command, "echo") == 0)
		return (ft_echo(args, shell), 1);
	if (ft_strcmp(command, "pwd") == 0)
	{
		result = ft_pwd();
		if (result == 1)
			shell->exit_status = 0;
		else
			shell->exit_status = 1;
		return (1);
	}
	if (ft_strcmp(command, "env") == 0)
		return (ft_env(args, shell), 1);
	if (ft_strcmp(command, "cd") == 0)
		return (ft_cd(args, i, shell), 1);
	if (ft_strcmp(command, "export") == 0)
		return (ft_export(args, shell), 1);
	if (ft_strcmp(command, "unset") == 0)
		return (ft_unset(args, &shell->vars, &shell->envp), 1);
	if (ft_strcmp(command, "exit") == 0)
		return (ft_exit(args, shell), 1);
	return (0);
}

int	child_builtin(int *i, t_shell *shell, t_command_data *data)
{
	int		original_stdin;
	int		original_stdout;
	char	*command;
	char	**args;

	original_stdin = dup(STDIN_FILENO);
	original_stdout = dup(STDOUT_FILENO);
	command = data->commands[*i];
	args = data->arguments[*i];
	if (handle_input_redirection(data, i, original_stdin, shell))
		return (1);
	if (handle_output_redirection(data, i, original_stdout, shell))
		return (1);
	execute_builtin_command(command, args, shell, i);
	restore_fds(original_stdin, original_stdout);
	return (1);
}
