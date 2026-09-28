#!/usr/bin/env bash
# Scans the files staged in a Git repo for likely secrets.
# Usage: secret-scan.sh [repo-dir]   Exit 0 = clean, 1 = possible secrets (printed), 2 = error.
set -uo pipefail

dir="${1:-.}"
cd "$dir" || exit 2

pattern='(AKIA[0-9A-Z]{16}|sk-ant-[A-Za-z0-9_-]{20,}|sk-[A-Za-z0-9]{32,}|gh[pousr]_[A-Za-z0-9]{36}|github_pat_[A-Za-z0-9_]{40,}|xox[abprs]-[A-Za-z0-9-]{10,}|AIza[0-9A-Za-z_-]{35}|-----BEGIN [A-Z ]*PRIVATE KEY-----|(api[_-]?key|secret|token|passw(or)?d)["'"'"']?[[:space:]]*[:=][[:space:]]*["'"'"'][^"'"'"'[:space:]]{12,}["'"'"'])'

found=0
while IFS= read -r -d '' file; do
  hits="$(git show ":$file" 2>/dev/null | grep -I -n -i -E "$pattern" || true)"
  if [ -n "$hits" ]; then
    found=1
    # Show file and line numbers only, never the secret itself.
    printf 'possible secret: %s (lines %s)\n' "$file" "$(printf '%s\n' "$hits" | cut -d: -f1 | paste -sd, -)"
  fi
done < <(git diff --cached --name-only --diff-filter=ACMR -z)

exit "$found"
