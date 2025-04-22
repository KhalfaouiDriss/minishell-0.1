// #include "../../include/minishell.h"

// void env_arg_inv(char *str)
// {
//     printf("env: '%s': No such file or directory\n", str);
// }

// void init_env(t_shell *shell, char **env)
// {
//     int i = 0;
//     char *name;
//     char *value;
//     char *equals_pos;

//     while (env[i])
//     {
//         equals_pos = ft_strchr(env[i], '=');
//         if (equals_pos)
//         {
//             int name_len = equals_pos - env[i];
//             name = malloc(name_len + 1);
//             if (!name)
//                 return;
            
//             ft_strncpy(name, env[i], name_len);
//             name[name_len] = '\0';
            
//             value = ft_strdup(equals_pos + 1);
            
//             add_env_var(shell, name, value);
            
//             free(name);
//             free(value);
//         }
//         i++;
//     }
// }

// void ft_env(t_shell *shell)
// {
//     char **args = shell->args;
//     t_env *head;

//     if (args[0] && ft_strncmp(args[0], "env",3) == 0)
//     {
//         if (args[1])
//             env_arg_inv(args[1]);
//         else
//         {
//             head = shell->env;
//             if (!head)
//             {
//                 printf("Environment is empty\n");
//                 return;
//             }
            
//             while (head)
//             {
//                 printf("%s=%s\n", head->name, head->value);
//                 head = head->next;
//             }
//         }
//     }
// }
