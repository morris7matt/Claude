#!/usr/bin/env bash
# Daily backup: commits each listed repo and pushes it if it has a remote.
# Refuses to commit anything that trips the secret scan. Writes one line per repo
# per run to the log, so a job that silently stopped shows up as missing entries.
#
# Repos: ~/.claude/skills, ~/.claude/scheduled-tasks, ~/.claude/commands (if present),
# plus one path per line in ~/.claude/backup-repos.txt (e.g. your project folders).
set -uo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
CLAUDE_DIR="${CLAUDE_CONFIG_DIR:-$HOME/.claude}"
LOG="${BACKUP_LOG:-$CLAUDE_DIR/backup.log}"
NOW="$(date '+%Y-%m-%d %H:%M:%S')"

log() { printf '%s  %-10s %s\n' "$NOW" "$1" "$2" >> "$LOG"; }

repos=("$CLAUDE_DIR/skills" "$CLAUDE_DIR/scheduled-tasks" "$CLAUDE_DIR/commands")
if [ -f "$CLAUDE_DIR/backup-repos.txt" ]; then
  while IFS= read -r line; do
    line="${line%%#*}"; line="$(printf '%s' "$line" | sed 's/[[:space:]]*$//')"
    [ -n "$line" ] && repos+=("${line/#\~/$HOME}")
  done < "$CLAUDE_DIR/backup-repos.txt"
fi

for repo in "${repos[@]}"; do
  [ -d "$repo" ] || continue
  if [ ! -d "$repo/.git" ]; then log SKIP "$repo (not a git repo)"; continue; fi

  git -C "$repo" add -A
  if git -C "$repo" diff --cached --quiet; then
    status="no-changes"
  elif ! "$SCRIPT_DIR/secret-scan.sh" "$repo" >> "$LOG" 2>&1; then
    git -C "$repo" reset -q
    log REFUSED "$repo (possible secret; nothing committed)"
    continue
  elif git -C "$repo" commit -q -m "Auto-backup $NOW"; then
    status="committed"
  else
    log ERROR "$repo (commit failed)"; continue
  fi

  if [ -n "$(git -C "$repo" remote)" ]; then
    if git -C "$repo" push -q 2>>"$LOG"; then status="$status+pushed"; else status="$status+PUSH-FAILED"; fi
  else
    status="$status (local only)"
  fi
  log OK "$repo $status"
done
