/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlos- <scarlos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 11:27:01 by scarlos-          #+#    #+#             */
/*   Updated: 2025/06/04 12:17:01 by scarlos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	setup_input_redirect(t_command_data *data, int i, t_shell *shell)
{
	int	fd;

	fd = -1;
	if (data->input_files && data->input_files[i] != NULL)
	{
		fd = open(data->input_files[i], O_RDONLY);
		if (fd < 0)
		{
			shell->exit_status = 1;
			exit(1);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
}

static void	redirect_and_close(int oldfd, int newfd)
{
	dup2(oldfd, newfd);
	close(oldfd);
}

static int	setup_output_redirect(t_command_data *data, int i, t_shell *shell)
{
	int	fd;
	int	flags;
	int	j;

	j = 0;
	if (data->num_out_redirs && data->num_out_redirs[i] > 0)
	{
		while (j < data->num_out_redirs[i])
		{
			flags = O_WRONLY | O_CREAT;
			if (data->out_redirs[i][j].append)
				flags |= O_APPEND;
			else
				flags |= O_TRUNC;
			fd = open(data->out_redirs[i][j].file, flags, 0666);
			if (fd < 0)
			{
				shell->exit_status = 1;
				return (-1);
			}
			redirect_and_close(fd, STDOUT_FILENO);
			j++;
		}
	}
	return (0);
}

void	setup_pipes_and_redirections(t_command_data *data, t_exec_state *state,
		int num_commands, t_shell *shell)
{
	if (state->i > 0)
	{
		dup2(state->prev_pipe_read, STDIN_FILENO);
		close(state->prev_pipe_read);
	}
	setup_input_redirect(data, state->i, shell);
	if (state->i < num_commands - 1)
	{
		dup2(state->pipefd[1], STDOUT_FILENO);
	}
	if (setup_output_redirect(data, state->i, shell) < 0)
		exit(shell->exit_status);
}

int	setup_pipeline(t_command_data *data, t_exec_state *state, t_shell *shell)
{
	int	result;

	result = 0;
	if (state->i < data->num_commands - 1)
	{
		result = pipe(state->pipefd);
		if (result == -1)
		{
			perror("minishell: pipe");
			shell->exit_status = 1;
			return (-1);
		}
	}
	if (data->heredoc_delim != NULL && state->i == 0)
	{
		state->heredoc_fd = setup_heredoc(data->heredoc_delim, shell,
				data->heredoc_quoted);
		if (state->heredoc_fd == -1)
		{
			shell->exit_status = 1;
			return (-1);
		}
	}
	return (0);
}
