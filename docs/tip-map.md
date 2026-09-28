# Tip map

Every tip in [Claude Workflow Tips](https://chadtimbl.in/writings/claude-workflow-tips) and how this kit handles it.

✅ built into this kit · ⚙️ one-time setting (see README) · 🧠 habit · ⏭️ optional / skip

## Working with Claude
| Tip | Status | How |
|---|---|---|
| Make Claude more concise | ✅ | Communication block in `global/CLAUDE.md` plus `outputStyle: "Concise"` in `global/settings.json`. If it drifts, say "reread your communication rules". |
| Guardrails & human in the loop | ✅ | "Guardrails" section of `global/CLAUDE.md`. Skills that send or publish get `disable-model-invocation: true`. |
| Show Claude images & screenshots | 🧠 | Paste them in, or drop them into `context/YYYY.MM.DD/`. |
| Structure prompts with XML tags | 🧠 | Use `<draft>…</draft>`, `<rules>`, `<example>`. Type both tags first, then fill in the middle. |
| Talk with dictation | 🧠 | Your OS's dictation, or Wispr Flow. |
| Listen to how others use Claude | 🧠 | Podcasts the article recommends: I'd Rather Be Writing, The Panel, Startup to Last, Love At First Try. |

## Setting up Claude Code
| Tip | Status | How |
|---|---|---|
| Use Claude Code for more than code | 🧠 | The whole kit assumes this. |
| Keep session history from auto-deleting | ✅ | `cleanupPeriodDays: 99999` |
| Organize sessions with custom groups | ⚙️ | README, One-time setup, step 3 |
| Separate desktop app per Claude account | ⏭️ | Only if you have work and personal accounts. On a Mac: `open -n -a "Claude" --args --user-data-dir="$HOME/Library/Application Support/Claude-Work"`. The author publishes a setup guide you can hand to Claude. |
| Add `.claude` to the Finder sidebar | ⚙️ | README, One-time setup, step 6 |

## Standing instructions
| Tip | Status | How |
|---|---|---|
| Global CLAUDE.md | ✅ | `global/CLAUDE.md`, installed to `~/.claude/CLAUDE.md`. Fill in "About me" on your machine only. |
| CLAUDE.md & project instructions | ✅ | `/new-project` template. Ask Claude to draft or refresh it. |
| Your own shorthand | ✅ | `ship`, `commit no push`, `wrap up`, `second opinion` in `global/CLAUDE.md`. Add your own. |
| Move Chat Projects into Code folders | ✅ | `/import-claude-export` |

## Keeping context in files
| Tip | Status | How |
|---|---|---|
| Default to Markdown | ✅ | Working rule in `global/CLAUDE.md`. Use VS Code's ⌘⇧V to read files rendered. Avoid keeping anything Claude must edit in Google Docs. |
| PROGRESS.md / TODO.md / HOMEWORK.md | ✅ | `/new-project` template; `/wrap-up` keeps them current. |
| README.md files for context | ✅ | Template README; `/wrap-up` step 2. |
| Dated `context/` subfolders | ✅ | `context/YYYY.MM.DD/` in the template |
| Transcripts as context | 🧠 | Save as `.md` in `context/YYYY.MM.DD/`, then ask for a summary and action items. |
| End sessions by updating context files | ✅ | `/wrap-up` (or say "wrap up") |
| Organized, consistent folders | ✅ | One folder per area under your Projects root; YYYY.MM.DD dates; rule to update references when moving files |
| Export your data for search | ✅ | `/import-claude-export` also searches an export. Re-export periodically into dated folders. |

## Building & changing things
| Tip | Status | How |
|---|---|---|
| Build your own apps & tools | 🧠 | Start small with something that scratches a real itch. |
| Have Claude check its own work | ✅ | Working rule in `global/CLAUDE.md` |
| Plan mode for bigger changes | 🧠 | Shift+Tab in the terminal, or Plan in the desktop mode picker. |
| Second opinion before running a plan | ✅ | `/second-opinion` |
| Let Claude run Git | ✅ | Shorthand; just ask in plain English. |
| Git in your Claude folders | ✅ | `install.sh` puts `~/.claude/skills`, `scheduled-tasks` and `commands` under Git. The secret scan runs before every commit. It does **not** version `~/.claude` itself, which holds your transcripts. Remotes stay private. |
| Work in an existing codebase | 🧠 | Open Code in the repo and describe the goal. |

## Skills, tools & automation
| Tip | Status | How |
|---|---|---|
| Reusable skills, slash commands & templates | ✅ | Six skills here; `new-project/template/` is a starter folder. Put project-only skills in `<project>/.claude/skills/` with a name prefix. |
| Self-improving skills | ✅ | `/reflect-improve <skill>` |
| Scheduled tasks | ⚙️ | Code sidebar > Routines > New routine. Local for file work, Cloud for web-only jobs. Email the results to yourself; hardcode the recipient. |
| Automations (Launch Agents) | ✅ | `scripts/schedule-backup.sh` + `scripts/backup.sh`. Ask Claude for others, such as a daily `git pull`. |
| Connectors | ⚙️ | README, One-time setup, step 5. Review community MCP servers before installing. |
| Edit Google Sheets & Docs | ⏭️ | Needs the author's Apps Script guide (about 15 minutes; the token grants full account access). Skip it unless you need Docs or Sheets. |

## Everyday uses
| Tip | Status | How |
|---|---|---|
| Writing partner | 🧠 | Draft, polish, pressure-test. Your voice rules go in "About me". |
| Plan day & week | ✅ | `/plan-day` |
| Research decisions | 🧠 | Turn on web search; give it your constraints. |
| Crunch data & spreadsheets | 🧠 | Drop the CSV into `context/`. |
| Organize files on your computer | 🧠 | Read-only audit first, ground rules, approved plan, count files before and after, tracker file. Search for references before moving a folder. |
| Vet suspicious emails & links | 🧠 | Treat it as a second opinion, not the final word. |
| Tedious forms | 🧠 | Claude checks and explains; you submit. |
| Consolidate Gmail filters | 🧠 | Export the filters XML, have Claude consolidate them, then import. Keep the original export. |
| Recover "unavailable" YouTube titles | ⏭️ | Niche. Uses Claude in Chrome and filmot.com. |
