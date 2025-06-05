/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pviegas- <pviegas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 03:45:00 by pviegas-          #+#    #+#             */
/*   Updated: 2025/06/05 04:24:07 by pviegas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	handle_input_redirect_logic(char **args, t_command_data *data,
		t_indices *indices, int cmd_index)
{
	int		fd;

	if (!get_shell()->is_counting)
	{
		if (get_shell()->exit_status == 0)
		{
			fd = open(args[indices->i + 1], O_RDONLY);
			if (fd < 0)
			{
				ft_putstr_fd("minishell: ", 2);
				perror(args[indices->i + 1]);
				get_shell()->exit_status = 1;
			}
			else
				close(fd);
		}
		if (data->input_files)
		{
			free(data->input_files[cmd_index]);
			data->input_files[cmd_index] = ft_strdup(args[indices->i + 1]);
			if (data->input_files[cmd_index] == NULL)
				get_shell()->exit_status = 1;
		}
	}
}

static void	handle_output_file_open(char **args, t_indices *indices,
		const char *token)
{
	int		fd;
	int		flags;
	t_shell	*shell;

	shell = get_shell();
	if (shell->exit_status == 0)
	{
		flags = O_WRONLY | O_CREAT;
		if (ft_strcmp(token, ">>") == 0)
			flags |= O_APPEND;
		else
			flags |= O_TRUNC;
		fd = open(args[indices->i + 1], flags, 0666);
		if (fd < 0)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(args[indices->i + 1]);
			shell->exit_status = 1;
		}
		else
			close(fd);
	}
}

static void	setup_output_redirection(t_command_data *data, char **args,
		t_indices *indices, int cmd_index)
{
	const char	*token;

	token = args[indices->i];
	data->out_redirs[cmd_index][data->num_out_redirs[cmd_index]].file
		= ft_strdup(args[indices->i + 1]);
	if (ft_strcmp(token, ">>") == 0)
		data->out_redirs[cmd_index][data->num_out_redirs[cmd_index]].append = 1;
	else
		data->out_redirs[cmd_index][data->num_out_redirs[cmd_index]].append = 0;
	data->num_out_redirs[cmd_index]++;
}

static void	handle_output_redirect_logic(char **args, t_command_data *data,
		t_indices *indices, int cmd_index)
{
	const char		*token;
	t_redirection	*new_redirs;
	t_shell			*shell;

	shell = get_shell();
	token = args[indices->i];
	if (!shell->is_counting && data->out_redirs && data->num_out_redirs)
	{
		handle_output_file_open(args, indices, token);
		new_redirs = realloc(data->out_redirs[cmd_index],
				(data->num_out_redirs[cmd_index] + 1) * sizeof(t_redirection));
		if (new_redirs == NULL)
		{
			shell->exit_status = 1;
			indices->i += 2;
			return ;
		}
		data->out_redirs[cmd_index] = new_redirs;
		setup_output_redirection(data, args, indices, cmd_index);
		if (data->out_redirs[cmd_index][data->num_out_redirs[cmd_index]
			- 1].file == NULL)
			shell->exit_status = 1;
	}
}

void	handle_redirect_error(const char *token, char **args,
		t_indices *indices, t_shell *shell)
{
	if (token == NULL || args[indices->i + 1] == NULL)
	{
		print_error_simple("syntax error near unexpected token `newline'",
			2, shell);
		indices->i++;
	}
	else if (is_operator(args[indices->i + 1]))
	{
		ft_putstr_fd("bash: syntax error near unexpected token `", 2);
		ft_putstr_fd(args[indices->i + 1], 2);
		ft_putstr_fd("'\n", 2);
		shell->exit_status = 2;
		indices->i++;
	}
}

void	handle_redirect(char **args, t_command_data *data, t_indices *indices,
	int cmd_index)
{
	const char	*token;
	t_shell		*shell;

	token = args[indices->i];
	shell = get_shell();
	handle_redirect_error(token, args, indices, shell);
	if (token == NULL || args[indices->i + 1] == NULL
		|| is_operator(args[indices->i + 1]))
		return ;
	if (ft_strcmp(token, "<") == 0)
	{
		handle_input_redirect_logic(args, data, indices, cmd_index);
		indices->i += 2;
	}
	else if (ft_strcmp(token, ">") == 0 || ft_strcmp(token, ">>") == 0)
	{
		handle_output_redirect_logic(args, data, indices, cmd_index);
		indices->i += 2;
	}
	else
	{
		print_error_simple("syntax error near redirect", 2, shell);
		indices->i++;
	}
}
