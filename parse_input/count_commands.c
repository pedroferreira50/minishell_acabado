/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlos- <scarlos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 11:57:31 by scarlos-          #+#    #+#             */
/*   Updated: 2025/06/04 11:57:32 by scarlos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	process_pipe_operator(t_count_context *ctx, t_indices *idx,
	int *command_index)
{
	handle_pipe_validation(ctx, idx);
	if (ctx->shell->exit_status == 2)
		return ;
	handle_pipe(ctx->data, command_index, ctx->shell);
	idx->j = 0;
	idx->i++;
}

static void	process_operator_token(t_count_context *ctx, t_indices *idx,
	int *command_index)
{
	if (ft_strcmp(ctx->args[idx->i], "|") == 0)
		process_pipe_operator(ctx, idx, command_index);
	else if (ft_strcmp(ctx->args[idx->i], "<<") == 0)
		process_heredoc_operator(ctx, idx);
	else
		process_redirect_operator(ctx, idx, *command_index);
}

static void	validate_final_state(t_command_data *data, t_shell *shell)
{
	if (data->num_commands <= data->num_pipes)
	{
		print_error_simple("invalid pipe sequence", 2, shell);
		data->num_commands = 0;
	}
}

static void	count_commands_internal(t_count_context *ctx)
{
	t_indices	idx;
	int			command_index;

	if (!validate_args_and_init(ctx, &idx, &command_index))
		return ;
	while (idx.i < (size_t)ctx->count)
	{
		if (!check_current_arg(ctx, idx.i))
			return ;
		if (is_operator(ctx->args[idx.i]))
			process_operator_token(ctx, &idx, &command_index);
		else
		{
			ctx->arg_counts[command_index]++;
			idx.j++;
			idx.i++;
		}
		if (ctx->shell->exit_status == 2)
			return ;
	}
	ctx->data->num_commands = command_index + 1;
	validate_final_state(ctx->data, ctx->shell);
}

void	count_commands(char **args, int count, t_command_data *data,
	int *arg_counts)
{
	t_count_context	ctx;
	t_shell			*shell;

	shell = get_shell();
	ctx = (t_count_context){args, count, data, arg_counts, shell};
	count_commands_internal(&ctx);
}
