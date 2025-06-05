/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pviegas- <pviegas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 07:29:00 by pviegas-          #+#    #+#             */
/*   Updated: 2025/06/05 02:18:45 by pviegas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_operator(const char *token)
{
	if (token == NULL)
		return (0);
	if (ft_strcmp(token, "|") == 0)
		return (1);
	if (ft_strcmp(token, "<") == 0)
		return (1);
	if (ft_strcmp(token, ">") == 0)
		return (1);
	if (ft_strcmp(token, ">>") == 0)
		return (1);
	if (ft_strcmp(token, "<<") == 0)
		return (1);
	return (0);
}

int	*initialize_arg_counts(int count, t_shell *shell)
{
	int	*arg_counts;

	arg_counts = malloc(count * sizeof(int));
	if (arg_counts == NULL)
	{
		shell->exit_status = 1;
		return (NULL);
	}
	ft_memset(arg_counts, 0, sizeof(int) * count);
	return (arg_counts);
}

void	finalize_arguments(t_command_data *data, int *arg_counts)
{
	int	i;

	i = 0;
	while (i < data->num_commands)
	{
		if (data->arguments[i] != NULL)
			data->arguments[i][arg_counts[i]] = NULL;
		i++;
	}
}

static void	handle_operator_wrapper(char **args, t_command_data *data,
	t_parse_state *state, t_shell *shell)
{
	if (args[state->idx.i] == NULL)
		return ;
	if (ft_strcmp(args[state->idx.i], "|") == 0)
	{
		state->command_index++;
		state->idx.j = 0;
		state->idx.i++;
		return ;
	}
	if (ft_strcmp(args[state->idx.i], "<<") == 0)
		handle_heredoc(args, data, &state->idx, shell);
	else
	{
		shell->is_counting = 0;
		handle_redirect(args, data, &state->idx, shell, state->command_index);
		if (shell->exit_status == 2)
			return ;
	}
}

void	populate_commands(char **args, int *arg_counts,
	t_command_data *data, t_shell *shell)
{
	t_parse_state	state;
	size_t			arg_count;

	state.command_index = 0;
	state.idx.i = 0;
	state.idx.j = 0;
	arg_count = 0;
	if (args == NULL)
	{
		shell->exit_status = 2;
		return ;
	}
	arg_count = count_args(args);
	while (state.idx.i < arg_count && args[state.idx.i] != NULL)
	{
		if (is_operator(args[state.idx.i]))
			handle_operator_wrapper(args, data, &state, shell);
		else
		{
			populate_command(args, arg_counts, data, &state);
			populate_argument(args, data, &state);
		}
	}
}
