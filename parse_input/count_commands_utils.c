/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_commands_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlos- <scarlos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 11:57:46 by scarlos-          #+#    #+#             */
/*   Updated: 2025/06/04 11:57:47 by scarlos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_pipe_validation(t_count_context *ctx, t_indices *idx)
{
	if (idx->j == 0)
	{
		if (idx->i == 0)
		{
			print_error_simple("syntax error: no command before pipe", 2,
				ctx->shell);
			return ;
		}
	}
	if (idx->i + 1 >= (size_t)ctx->count || ctx->args[idx->i + 1] == NULL)
	{
		print_error_simple("syntax error near token", 2, ctx->shell);
		return ;
	}
	if (idx->i + 1 < (size_t)ctx->count && is_operator(ctx->args[idx->i + 1])
		&& ft_strcmp(ctx->args[idx->i + 1], "|") == 0)
	{
		print_error_simple("syntax error near unexpected token", 2,
			ctx->shell);
		return ;
	}
}

void	process_heredoc_operator(t_count_context *ctx, t_indices *idx)
{
	if (idx->i + 1 >= (size_t)ctx->count || ctx->args[idx->i + 1] == NULL)
	{
		print_error_simple("syntax error near token", 2, ctx->shell);
		return ;
	}
	idx->i += 2;
}

void	process_redirect_operator(t_count_context *ctx, t_indices *idx,
	int command_index)
{
	t_redirect_context	redirect_ctx;

	if (idx->i + 1 >= (size_t)ctx->count || ctx->args[idx->i + 1] == NULL)
	{
		print_error_simple("syntax error near token", 2, ctx->shell);
		return ;
	}
	ctx->shell->is_counting = 1;
	redirect_ctx.args = ctx->args;
	redirect_ctx.data = ctx->data;
	redirect_ctx.indices = idx;
	redirect_ctx.shell = ctx->shell;
	redirect_ctx.cmd_index = command_index;
	handle_redirect(&redirect_ctx);
}

int	validate_args_and_init(t_count_context *ctx, t_indices *idx,
	int *command_index)
{
	idx->i = 0;
	idx->j = 0;
	*command_index = 0;
	ctx->data->num_pipes = 0;
	if (ctx->args == NULL)
	{
		ctx->shell->exit_status = 2;
		return (0);
	}
	return (1);
}

int	check_current_arg(t_count_context *ctx, size_t i)
{
	if (ctx->args[i] == NULL)
	{
		ctx->shell->exit_status = 2;
		return (0);
	}
	return (1);
}
