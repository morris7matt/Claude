# Claude Code daily-driver kit

A starter kit for moving daily work from Cowork into **Claude Code**. It follows Chad Timblin's [Claude Workflow Tips](https://chadtimbl.in/writings/claude-workflow-tips) (last updated Sept 27, 2026). The core idea: do almost everything in Claude Code, even work that isn't code, inside ordinary folders you own. Keep Claude Chat for quick questions only.

[`docs/tip-map.md`](docs/tip-map.md) lists every tip from the article and says whether this kit builds it, it's a one-time setting, or it's a habit.

> ⚠️ **This repo is public.** It holds only templates. Your filled-in `~/.claude/CLAUDE.md`, your skills, and your scheduled tasks stay on your machine, in their own local Git repos. If you give those repos a remote, make it **private**.

## What's in here

| Path | What it is |
|---|---|
| `global/CLAUDE.md` | Global instructions template: concise-reply rules, guardrails, working rules, and shorthand (`ship`, `commit no push`, `wrap up`, `second opinion`) |
| `global/settings.json` | `cleanupPeriodDays: 99999` (keep session history) and `outputStyle: "Concise"` |
| `skills/wrap-up` | `/wrap-up`: end a session by updating CLAUDE.md, PROGRESS.md, TODO.md and READMEs |
| `skills/second-opinion` | `/second-opinion`: a subagent with no conversation context does an adversarial review of a plan or skill |
| `skills/reflect-improve` | `/reflect-improve <skill>`: adds a suggest-only friction log to a skill so it proposes its own improvements |
| `skills/new-project` | `/new-project <name>`: creates a folder with CLAUDE.md, PROGRESS.md, TODO.md, `context/YYYY.MM.DD/`, and Git |
| `skills/import-claude-export` | `/import-claude-export <export dir>`: turns a claude.ai data export into one Claude Code folder per Chat Project (plus whatever Cowork sessions can be recovered) |
| `skills/plan-day` | `/plan-day`: evening planning that checks tasks against your real free time on the calendar |
| `scripts/install.sh` | Installs all of the above into `~/.claude`. Never overwrites anything and backs up `settings.json` first |
| `scripts/backup.sh` | Commits (and pushes, if a remote is set) your skills and scheduled-tasks repos. Refuses to commit if the secret scan finds anything. Writes one log line per repo |
| `scripts/schedule-backup.sh` | Runs `backup.sh` daily: a Launch Agent on macOS, cron on Linux |

## Quick start (on your own computer, not a cloud session)

```bash
git clone https://github.com/morris7matt/Claude.git ~/claude-kit
~/claude-kit/scripts/install.sh
~/claude-kit/scripts/schedule-backup.sh 21:00   # optional daily backup
```

Then open Claude Code in any folder and say: *"Fill in the About me section of my global CLAUDE.md. Interview me."*

On Windows, run the scripts from Git Bash. For the daily backup, `schedule-backup.sh` prints a `schtasks` command to use instead.

## One-time setup (app settings the scripts can't change)

1. **Concise output style.** `install.sh` adds this to `settings.json` unless you already set a style. In the desktop app it's under Settings > Claude Code > Output style.
2. **Claude Chat preferences.** Code never sees the "Instructions for Claude" you set on claude.ai. Copy any that matter into `~/.claude/CLAUDE.md`.
3. **Sidebar groups.** In the Code sidebar's filter menu, group sessions by *Custom groups* and create a few: 🟢 Active, ⏸️ Paused, ✴️ Waiting, plus at most one topic group. Pin the sessions you keep coming back to.
4. **Keep computer awake.** Turn this on under Settings > Desktop app > System so local scheduled tasks don't miss their runs.
5. **Connectors.** Connect the tools you use most (calendar, Drive, task manager). `/plan-day` uses them if they're available.
6. **Easy access to `~/.claude` (macOS).** In Finder, press ⌘⇧H, then ⌘⇧. to show hidden files. Drag `.claude` to the sidebar, then press ⌘⇧. again to hide them.
7. **Your existing skills.** Skills you added to your Claude account (your Fusion 360 and Kobra X experts, for example) already appear in Claude Code. Local skills now live in `~/.claude/skills/`, which the installer puts under Git.

## Moving from Cowork to Code

1. **Get your Cowork sessions out first.** They are *not* in the claude.ai export. They exist only in the desktop app's data folder.
2. **Export your data.** Go to Settings > Privacy > Export data. The download link expires after 24 hours. Unzip every part of the export into one dated folder.
3. In Claude Code, run `/import-claude-export <that folder>`. It lists your Projects and asks which to migrate. For each one it creates a folder with a lean CLAUDE.md, a memory snapshot, a chat index, and a verbatim copy of every chat. It also offers to recover Cowork transcripts from the app's data folder (it only reads that folder, and asks before copying anything).
4. Keep one Chat Project, such as "Misc. Questions", for anything you'd otherwise search Google for. Everything else starts from a folder.
5. Turn the recurring routines you've been running in Cowork into skills: *"Turn this into a skill in ~/.claude/skills."* Then run `/second-opinion` on the skill before its first run, and `/reflect-improve` on it once it works.
6. Recurring jobs become **Routines** (Code sidebar > Routines > New routine). Choose **Local** if the job needs your files, **Cloud** if it only needs the web and connectors. Have them email the results to you.

## The daily loop

- **Start:** open a session in the right project folder (new area → `/new-project`). Claude reads that folder's CLAUDE.md and PROGRESS.md.
- **Drop context in** `context/YYYY.MM.DD/`: screenshots, transcripts, exports.
- **Bigger changes:** use Plan mode, then say *second opinion*, then approve.
- **Finish:** say *wrap up*, then *ship* or *commit no push*.
- **Nightly:** `backup.sh` commits your skills and scheduled tasks. Add project folders to `~/.claude/backup-repos.txt`, one path per line.
- **Replies getting wordy in a long session?** Say *"reread your communication rules"* or start a new session.
