/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_commands_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pviegas- <pviegas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 11:57:46 by scarlos-          #+#    #+#             */
/*   Updated: 2025/06/05 02:49:07 by pviegas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_pipe_logic(char **args, int count, t_indices *idx,
	int *command_index)
{
	t_shell	*shell;

	(void)command_index;
	shell = get_shell();
	if (idx->j == 0)
	{
		if (idx->i == 0)
		{
			print_error_simple("syntax error: no command before pipe",
				2, shell);
			return ;
		}
	}
	if (idx->i + 1 >= (size_t)count || args[idx->i + 1] == NULL)
	{
		print_error_simple("syntax error near token", 2, shell);
		return ;
	}
	if (idx->i + 1 < (size_t)count && is_operator(args[idx->i + 1])
		&& ft_strcmp(args[idx->i + 1], "|") == 0)
	{
		print_error_simple("syntax error near unexpected token", 2, shell);
		return ;
	}
}

void	handle_heredoc_logic(char **args, int count, t_indices *idx)
{
	if (idx->i + 1 >= (size_t)count || args[idx->i + 1] == NULL)
	{
		print_error_simple("syntax error near token", 2, get_shell());
		return ;
	}
	idx->i += 2;
}

void	handle_redirect_logic(char **args, int count, t_command_data *data,
	t_indices *idx)
{
	t_shell	*shell;

	(void)data;
	shell = get_shell();
	if (idx->i + 1 >= (size_t)count || args[idx->i + 1] == NULL)
	{
		print_error_simple("syntax error near token", 2, shell);
		return ;
	}
	shell->is_counting = 1;
}

void	process_pipe_operator(char **args, int count, t_command_data *data,
	t_indices *idx)
{
	t_shell		*shell;
	static int	command_index = 0;

	shell = get_shell();
	handle_pipe_logic(args, count, idx, &command_index);
	if (shell->exit_status == 2)
		return ;
	handle_pipe(data, &command_index, shell);
	idx->j = 0;
	idx->i++;
}

void	process_heredoc_operator(char **args, int count, t_indices *idx)
{
	t_shell	*shell;

	shell = get_shell();
	handle_heredoc_logic(args, count, idx);
	if (shell->exit_status == 2)
		return ;
}

void	process_redirect_operator(char **args, int count, t_command_data *data,
	t_indices *idx)
{
	t_shell	*shell;
	int		command_index;

	shell = get_shell();
	command_index = 0;
	handle_redirect_logic(args, count, data, idx);
	if (shell->exit_status == 2)
		return ;
	handle_redirect(args, data, idx, shell, command_index);
}
