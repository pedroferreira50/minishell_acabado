/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pviegas- <pviegas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 08:02:00 by pedro             #+#    #+#             */
/*   Updated: 2025/06/04 08:23:52 by pviegas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	validate_redirect_syntax(char **args, t_indices *indices, t_shell *shell)
{
	const char	*token;

	token = args[indices->i];
	if (token == NULL || args[indices->i + 1] == NULL)
	{
		print_error_simple("syntax error near unexpected token `newline'",
			2, shell);
		indices->i++;
		return (-1);
	}
	if (is_operator(args[indices->i + 1]))
	{
		ft_putstr_fd("bash: syntax error near unexpected token `", 2);
		ft_putstr_fd(args[indices->i + 1], 2);
		ft_putstr_fd("'\n", 2);
		shell->exit_status = 2;
		indices->i++;
		return (-1);
	}
	return (0);
}

int	handle_input_redirect(t_redirect_params *params, t_shell *shell)
{
	if (!shell->is_counting)
	{
		process_input_file(params, shell);
		update_input_files(params, shell);
	}
	params->indices->i += 2;
	return (0);
}

int	open_output_file(char **args, t_indices *indices,
	const char *token, t_shell *shell)
{
	int	fd;
	int	flags;

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
			return (-1);
		}
		else
			close(fd);
	}
	return (0);
}

int	add_output_redirection(t_output_params *params)
{
	t_redirection	*new_redirs;
	int				cmd_idx;
	int				num_redirs;

	cmd_idx = params->base.cmd_index;
	num_redirs = params->base.data->num_out_redirs[cmd_idx];
	new_redirs = realloc(params->base.data->out_redirs[cmd_idx],
			(num_redirs + 1) * sizeof(t_redirection));
	if (new_redirs == NULL)
		return (-1);
	params->base.data->out_redirs[cmd_idx] = new_redirs;
	return (set_redirection_data(params));
}

int	handle_output_redirect(t_output_params *params, t_shell *shell)
{
	if (!shell->is_counting && params->base.data->out_redirs
		&& params->base.data->num_out_redirs)
	{
		if (open_output_file(params->base.args, params->base.indices,
				params->token, shell) == -1)
		{
			params->base.indices->i += 2;
			return (-1);
		}
		if (add_output_redirection(params) == -1)
		{
			shell->exit_status = 1;
			params->base.indices->i += 2;
			return (-1);
		}
	}
	params->base.indices->i += 2;
	return (0);
}
