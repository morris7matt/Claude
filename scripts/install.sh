#!/usr/bin/env bash
# Installs this starter kit into ~/.claude without overwriting anything:
#   - global CLAUDE.md  (copied only if you don't have one; otherwise saved alongside as CLAUDE.md.starter)
#   - settings.json     (keys merged in; your existing keys win; original backed up)
#   - skills            (each copied only if a skill of that name doesn't exist yet)
#   - Git safety net on ~/.claude/skills and ~/.claude/scheduled-tasks (local-only, no remote)
set -euo pipefail

REPO_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
CLAUDE_DIR="${CLAUDE_CONFIG_DIR:-$HOME/.claude}"
STAMP="$(date +%Y.%m.%d-%H%M%S)"

say() { printf '%s\n' "$*"; }

mkdir -p "$CLAUDE_DIR/skills"

# --- Global CLAUDE.md ---------------------------------------------------------
if [ ! -e "$CLAUDE_DIR/CLAUDE.md" ]; then
  cp "$REPO_DIR/global/CLAUDE.md" "$CLAUDE_DIR/CLAUDE.md"
  say "CLAUDE.md: installed to $CLAUDE_DIR/CLAUDE.md — fill in the 'About me' section."
else
  cp "$REPO_DIR/global/CLAUDE.md" "$CLAUDE_DIR/CLAUDE.md.starter"
  say "CLAUDE.md: you already have one; left it alone. Starter saved as CLAUDE.md.starter — ask Claude to merge the parts you want."
fi

# --- settings.json (merge, existing values win) -------------------------------
SETTINGS="$CLAUDE_DIR/settings.json"
if command -v python3 >/dev/null 2>&1; then
  if [ -e "$SETTINGS" ]; then cp "$SETTINGS" "$SETTINGS.bak-$STAMP"; fi
  python3 - "$SETTINGS" "$REPO_DIR/global/settings.json" <<'PY'
import json, os, sys
target, starter = sys.argv[1], sys.argv[2]
current = {}
if os.path.exists(target):
    with open(target) as f:
        text = f.read().strip()
    current = json.loads(text) if text else {}
with open(starter) as f:
    new = json.load(f)
added = [k for k in new if k not in current]
skipped = [k for k in new if k in current and current[k] != new[k]]
for k in added:
    current[k] = new[k]
with open(target, "w") as f:
    json.dump(current, f, indent=2)
    f.write("\n")
print("settings.json: added " + (", ".join(added) or "nothing"))
if skipped:
    print("settings.json: kept your existing values for " + ", ".join(skipped))
PY
else
  say "settings.json: python3 not found — add these keys by hand:"
  cat "$REPO_DIR/global/settings.json"
fi

# --- Skills -------------------------------------------------------------------
for src in "$REPO_DIR"/skills/*/; do
  name="$(basename "$src")"
  if [ -e "$CLAUDE_DIR/skills/$name" ]; then
    say "skill /$name: already exists, skipped"
  else
    cp -R "$src" "$CLAUDE_DIR/skills/$name"
    say "skill /$name: installed"
  fi
done

# --- Git safety net -----------------------------------------------------------
init_repo() {
  local dir="$1"
  [ -d "$dir" ] || return 0
  if [ ! -d "$dir/.git" ]; then
    git -C "$dir" init -q
    say "git: initialized $dir"
  fi
  if [ ! -e "$dir/.gitignore" ]; then
    printf '%s\n' '.DS_Store' '__pycache__/' '*.pyc' '*.log' 'data/' '.env' > "$dir/.gitignore"
  fi
  git -C "$dir" add -A
  if git -C "$dir" diff --cached --quiet; then
    say "git: $dir has nothing new to commit"
  elif "$REPO_DIR/scripts/secret-scan.sh" "$dir"; then
    git -C "$dir" commit -q -m "Snapshot $STAMP" && say "git: committed snapshot in $dir" \
      || say "git: commit failed in $dir (set git user.name / user.email, then rerun)"
  else
    git -C "$dir" reset -q
    say "git: NOT committing $dir — possible secrets found (see above)."
  fi
}
init_repo "$CLAUDE_DIR/skills"
init_repo "$CLAUDE_DIR/scheduled-tasks"
if [ -d "$CLAUDE_DIR/commands" ]; then init_repo "$CLAUDE_DIR/commands"; fi

say ""
say "Done. Next: finish the one-time app settings in README.md (section 'One-time setup')."
say "Optional daily backup of these repos: scripts/schedule-backup.sh"
