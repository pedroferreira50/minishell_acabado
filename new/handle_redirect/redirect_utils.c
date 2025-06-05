/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pviegas- <pviegas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 03:47:00 by pviegas-          #+#    #+#             */
/*   Updated: 2025/06/05 03:57:57 by pviegas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	restore_fds(int original_stdin, int original_stdout)
{
	dup2(original_stdin, STDIN_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdin);
	close(original_stdout);
}

int	handle_input_redirection(t_command_data *data, int *i, int original_stdin,
		t_shell *shell)
{
	int	fd_in;

	fd_in = -1;
	if (data->input_files && data->input_files[*i])
	{
		fd_in = open(data->input_files[*i], O_RDONLY);
		if (fd_in == -1)
		{
			perror("open input file");
			shell->exit_status = 1;
			restore_fds(original_stdin, STDOUT_FILENO);
			return (1);
		}
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
	}
	return (0);
}

static int	open_and_redirect_output(t_command_data *data, int *i, int j,
		int original_stdout)
{
	int	fd_out;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (data->out_redirs[*i][j].append)
		flags = flags | O_APPEND;
	else
		flags = flags | O_TRUNC;
	fd_out = open(data->out_redirs[*i][j].file, flags, 0666);
	if (fd_out == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(data->out_redirs[*i][j].file, 2);
		ft_putstr_fd(": Failed to open file\n", 2);
		restore_fds(STDIN_FILENO, original_stdout);
		return (1);
	}
	dup2(fd_out, STDOUT_FILENO);
	close(fd_out);
	return (0);
}

int	handle_output_redirection(t_command_data *data, int *i,
		int original_stdout, t_shell *shell)
{
	int	j;

	j = 0;
	if (data->num_out_redirs && data->num_out_redirs[*i] > 0)
	{
		while (j < data->num_out_redirs[*i])
		{
			if (open_and_redirect_output(data, i, j, original_stdout) == 1)
			{
				shell->exit_status = 1;
				return (1);
			}
			j++;
		}
	}
	return (0);
}
