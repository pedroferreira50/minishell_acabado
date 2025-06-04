/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlos- <scarlos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 07:26:00 by pedro             #+#    #+#             */
/*   Updated: 2025/06/04 11:58:23 by scarlos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_args(char **args)
{
	int	i;

	if (args == NULL)
	{
		return ;
	}
	i = 0;
	while (args[i] != NULL)
	{
		i++;
	}
}

static void	alloc_commands(t_command_data *data, t_shell *shell)
{
	int	i;

	data->commands = malloc(data->num_commands * sizeof(char *));
	data->arguments = malloc(data->num_commands * sizeof(char **));
	data->input_files = malloc(data->num_commands * sizeof(char *));
	data->out_redirs = malloc(data->num_commands * sizeof(t_redirection *));
	data->num_out_redirs = malloc(data->num_commands * sizeof(int));
	if (data->commands == NULL || data->arguments == NULL
		|| data->input_files == NULL || data->out_redirs == NULL
		|| data->num_out_redirs == NULL)
	{
		shell->exit_status = 1;
		free_command_data(data);
		return ;
	}
	ft_memset(data->commands, 0, data->num_commands * sizeof(char *));
	ft_memset(data->arguments, 0, data->num_commands * sizeof(char **));
	ft_memset(data->input_files, 0, data->num_commands * sizeof(char *));
	ft_memset(data->out_redirs, 0,
		data->num_commands * sizeof(t_redirection *));
	ft_memset(data->num_out_redirs, 0, data->num_commands * sizeof(int));
	i = 0;
	while (i < data->num_commands)
		data->num_out_redirs[i++] = 0;
}

void	populate_command(char **args, int *arg_counts,
	t_command_data *data, t_parse_state *state)
{
	if (state->idx.j == 0)
	{
		data->commands[state->command_index] = ft_strdup(args[state->idx.i]);
		data->arguments[state->command_index] = malloc((arg_counts[
					state->command_index] + 1) * sizeof(char *));
		if (data->commands[state->command_index] == NULL
			|| data->arguments[state->command_index] == NULL)
			return ;
	}
}

void	populate_argument(char **args, t_command_data *data,
	t_parse_state *state)
{
	if (state->idx.j == 0 && args[state->idx.i] && !*args[state->idx.i])
	{
		while (args[state->idx.i] && !*args[state->idx.i])
			state->idx.i++;
		if (!args[state->idx.i])
			return ;
		if (data->commands[state->command_index])
			free(data->commands[state->command_index]);
		data->commands[state->command_index] = ft_strdup(args[state->idx.i]);
	}
	data->arguments[state->command_index][state->idx.j]
		= ft_strdup(args[state->idx.i]);
	if (data->arguments[state->command_index][state->idx.j] == NULL)
		return ;
	state->idx.j++;
	state->idx.i++;
}

void	parse_input(char **args, int count, t_command_data *data,
	t_shell *shell)
{
	int	*arg_counts;

	ft_memset(data, 0, sizeof(t_command_data));
	arg_counts = initialize_arg_counts(count, shell);
	if (arg_counts == NULL)
		return ;
	count_commands(args, count, data, arg_counts);
	if (data->num_commands == 0)
	{
		free_command_data(data);
		free(arg_counts);
		return ;
	}
	alloc_commands(data, shell);
	if (data->commands == NULL || data->arguments == NULL)
	{
		free(arg_counts);
		return ;
	}
	populate_commands(args, arg_counts, data, shell);
	finalize_arguments(data, arg_counts);
	free(arg_counts);
}
