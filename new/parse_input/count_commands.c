/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pviegas- <pviegas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 11:57:31 by scarlos-          #+#    #+#             */
/*   Updated: 2025/06/05 04:07:06 by pviegas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	handle_pipe_operator(char **args, int count, t_indices *idx)
{
	t_shell	*shell;

	shell = get_shell();
	if (idx->j == 0)
	{
		if (idx->i == 0)
		{
			print_error_simple("syntax error: no command before pipe", 2,
				shell);
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

static void	handle_heredoc_operator(char **args, int count, t_indices *idx)
{
	if (idx->i + 1 >= (size_t)count || args[idx->i + 1] == NULL)
	{
		print_error_simple("syntax error near token", 2, get_shell());
		return ;
	}
	idx->i += 2;
}

static void	handle_redirect_operator(char **args, int count,
		t_command_data *data, t_indices *idx)
{
	t_shell	*shell;

	shell = get_shell();
	if (idx->i + 1 >= (size_t)count || args[idx->i + 1] == NULL)
	{
		print_error_simple("syntax error near token", 2, shell);
		return ;
	}
	shell->is_counting = 1;
	handle_redirect(args, data, idx, 0);
}

static int	initialize_count_data(char **args, t_command_data *data,
	t_parse_state *state)
{
	t_shell	*shell;

	shell = get_shell();
	state->idx.i = 0;
	state->idx.j = 0;
	state->command_index = 0;
	data->num_pipes = 0;
	if (args == NULL)
	{
		shell->exit_status = 2;
		return (1);
	}
	return (0);
}

static int	process_pipe_token(char **args, int count, t_command_data *data,
	t_parse_state *state)
{
	t_shell	*shell;

	shell = get_shell();
	handle_pipe_operator(args, count, &state->idx);
	if (shell->exit_status == 2)
		return (1);
	handle_pipe(data, &state->command_index, shell);
	state->idx.j = 0;
	state->idx.i++;
	return (0);
}

static int	process_operator_token(char **args, int count, t_command_data *data,
	t_parse_state *state)
{
	t_shell	*shell;

	shell = get_shell();
	if (ft_strcmp(args[state->idx.i], "|") == 0)
		return (process_pipe_token(args, count, data, state));
	else if (ft_strcmp(args[state->idx.i], "<<") == 0)
	{
		handle_heredoc_operator(args, count, &state->idx);
		if (shell->exit_status == 2)
			return (1);
	}
	else
	{
		handle_redirect_operator(args, count, data, &state->idx);
		if (shell->exit_status == 2)
			return (1);
	}
	return (0);
}

static void	finalize_count_data(t_command_data *data, int command_index)
{
	t_shell	*shell;

	shell = get_shell();
	data->num_commands = command_index + 1;
	if (data->num_commands <= data->num_pipes)
	{
		print_error_simple("invalid pipe sequence", 2, shell);
		data->num_commands = 0;
	}
}

void	count_commands(char **args, int count, t_command_data *data,
	int *arg_counts)
{
	t_parse_state	state;

	if (initialize_count_data(args, data, &state))
		return ;
	while (state.idx.i < (size_t)count)
	{
		if (args[state.idx.i] == NULL)
		{
			get_shell()->exit_status = 2;
			return ;
		}
		if (is_operator(args[state.idx.i]))
		{
			if (process_operator_token(args, count, data, &state))
				return ;
		}
		else
		{
			arg_counts[state.command_index]++;
			state.idx.j++;
			state.idx.i++;
		}
	}
	finalize_count_data(data, state.command_index);
}
