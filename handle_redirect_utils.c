/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirect_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlos- <scarlos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 11:19:57 by scarlos-          #+#    #+#             */
/*   Updated: 2025/06/04 11:19:58 by scarlos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_redirect(t_redirect_context *ctx)
{
	const char			*token;
	t_redirect_params	params;
	t_output_params		out_params;

	token = ctx->args[ctx->indices->i];
	if (validate_redirect_syntax(ctx->args, ctx->indices, ctx->shell) == -1)
		return ;
	params.args = ctx->args;
	params.data = ctx->data;
	params.indices = ctx->indices;
	params.cmd_index = ctx->cmd_index;
	if (ft_strcmp(token, "<") == 0)
		handle_input_redirect(&params, ctx->shell);
	else if (ft_strcmp(token, ">") == 0 || ft_strcmp(token, ">>") == 0)
	{
		out_params.base = params;
		out_params.token = token;
		handle_output_redirect(&out_params, ctx->shell);
	}
	else
	{
		print_error_simple("syntax error near redirect", 2, ctx->shell);
		ctx->indices->i++;
	}
}

int	set_redirection_data(t_output_params *params)
{
	int	idx;

	idx = params->base.data->num_out_redirs[params->base.cmd_index];
	params->base.data->out_redirs[params->base.cmd_index][idx].file
		= ft_strdup(params->base.args[params->base.indices->i + 1]);
	if (ft_strcmp(params->token, ">>") == 0)
		params->base.data->out_redirs[params->base.cmd_index][idx].append = 1;
	else
		params->base.data->out_redirs[params->base.cmd_index][idx].append = 0;
	if (params->base.data->out_redirs[params->base.cmd_index][idx].file == NULL)
		return (-1);
	params->base.data->num_out_redirs[params->base.cmd_index]++;
	return (0);
}

int	process_input_file(t_redirect_params *params, t_shell *shell)
{
	int	fd;

	if (shell->exit_status == 0)
	{
		fd = open(params->args[params->indices->i + 1], O_RDONLY);
		if (fd < 0)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(params->args[params->indices->i + 1]);
			shell->exit_status = 1;
		}
		else
			close(fd);
	}
	return (0);
}

int	update_input_files(t_redirect_params *params, t_shell *shell)
{
	if (params->data->input_files)
	{
		free(params->data->input_files[params->cmd_index]);
		params->data->input_files[params->cmd_index]
			= ft_strdup(params->args[params->indices->i + 1]);
		if (params->data->input_files[params->cmd_index] == NULL)
			shell->exit_status = 1;
	}
	return (0);
}
