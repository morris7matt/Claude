# claude-kit

Public starter kit that installs global Claude Code instructions, settings, skills and backup scripts into `~/.claude`. See README.md; `docs/tip-map.md` maps each source tip to where it's implemented.

## Rules for this repo
- **Public repo:** never commit personal facts, filled-in CLAUDE.md content, tokens, or anything from `~/.claude`. Templates and placeholders only.
- `scripts/install.sh` must stay non-destructive: never overwrite an existing CLAUDE.md, skill, or settings key; back up before merging.
- When adding a skill under `skills/`, add a row to the README table and `docs/tip-map.md`.
- Test script changes against a throwaway `HOME` (e.g. `HOME=$(mktemp -d) scripts/install.sh`) and `bash -n scripts/*.sh`.
