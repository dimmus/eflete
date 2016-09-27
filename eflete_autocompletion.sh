# bash completion for eflete
_eflete()
{
  local cur=${COMP_WORDS[COMP_CWORD]}
  local prev=${COMP_WORDS[COMP_CWORD-1]}

  case "$prev" in
   --id | --sd | --fd | --dd | --path | -i | -s | -f | -d )
     _filedir -d
     return 0
     ;;
   --reopen | --help | --license | --copyright | --version | -r | -h )
     return 0
     ;;
   --widget | -w )
     return 0
     ;;
   * )
     _filedir "edj|edc|pro"
     ;;
  esac

  # completing an option
  if [[ "$cur" == -* ]]; then
          COMPREPLY=( $( compgen -W "--name --path --export-edj --export-edc \
                      --id --sd --fd --dd --widget --reopen --version \
                      --copyright --license --help" -- $cur ) )
  fi
}
complete -F _eflete eflete
