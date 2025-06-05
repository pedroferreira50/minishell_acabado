#include "minishell.h"

static char	*get_env_value(t_shell *shell, const char *name)
{
	int		i;
	size_t	len;

	i = 0;
	len = strlen(name);
	while (shell->envp && shell->envp[i])
	{
		if (strncmp(shell->envp[i], name, len) == 0
			&& shell->envp[i][len] == '=')
			return (shell->envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

static void	set_shlvl(t_shell *shell)
{
	char	*shlvl_value;
	int		shlvl;
	char	shlvl_buf[16];

	shlvl_value = get_env_value(shell, "SHLVL");
	shlvl = 1;
	if (shlvl_value)
		shlvl = atoi(shlvl_value) + 1;
	snprintf(shlvl_buf, sizeof(shlvl_buf), "%d", shlvl);
	update_env_var("SHLVL", shlvl_buf, shell);
}

void	init_shell(t_shell *shell, char **envp)
{
	shell->envp = copy_envp(envp);
	if (!shell->envp)
	{
		shell->exit_status = 1;
		return ;
	}
	shell->vars = NULL;
	shell->exit_status = 0;
	g_signal = 0;
	//handle_export_without_value("OLDPWD", shell);
	set_shlvl(shell);
	setup_signals();
}
