/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_handling.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlos- <scarlos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 11:19:30 by scarlos-          #+#    #+#             */
/*   Updated: 2025/06/04 11:19:02 by scarlos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_input_files(t_command_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_commands)
	{
		if (data->input_files[i])
			free(data->input_files[i]);
		i++;
	}
	free(data->input_files);
	data->input_files = NULL;
}

static void	free_out_redirs_loop(t_redirection **out_redirs, \
			int *num_out_redirs, int num_cmds)
{
	int	i;
	int	j;

	i = 0;
	while (i < num_cmds)
	{
		j = 0;
		if (out_redirs[i])
		{
			while (j < num_out_redirs[i])
			{
				if (out_redirs[i][j].file)
					free(out_redirs[i][j].file);
				j++;
			}
			free(out_redirs[i]);
		}
		i++;
	}
}

static void	free_out_redirs(t_command_data *data)
{
	if (data->out_redirs)
	{
		free_out_redirs_loop(data->out_redirs, data->num_out_redirs,
			data->num_commands);
		free(data->out_redirs);
		data->out_redirs = NULL;
	}
	if (data->num_out_redirs)
	{
		free(data->num_out_redirs);
		data->num_out_redirs = NULL;
	}
}

void	free_command_data(t_command_data *data)
{
	if (!data)
		return ;
	if (data->commands)
		free_data_commands(data->commands, data->num_commands);
	if (data->arguments)
		free_data_arguments(data->arguments, data->num_commands);
	if (data->input_files)
		free_input_files(data);
	if (data->heredoc_delim)
	{
		free(data->heredoc_delim);
		data->heredoc_delim = NULL;
	}
	free_out_redirs(data);
	data->num_commands = 0;
	data->num_pipes = 0;
}
