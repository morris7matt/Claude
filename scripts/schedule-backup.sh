#!/usr/bin/env bash
# Schedules scripts/backup.sh to run daily (macOS: Launch Agent; Linux: crontab).
# Usage: schedule-backup.sh [HH:MM]     default 21:00
#        schedule-backup.sh --remove
set -euo pipefail

SCRIPT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/backup.sh"
LABEL="com.claude-kit.backup"
TIME="${1:-21:00}"

case "$(uname -s)" in
  Darwin)
    PLIST="$HOME/Library/LaunchAgents/$LABEL.plist"
    launchctl bootout "gui/$(id -u)/$LABEL" 2>/dev/null || true
    if [ "${1:-}" = "--remove" ]; then rm -f "$PLIST"; echo "Removed $LABEL."; exit 0; fi
    HOUR="$((10#${TIME%%:*}))"; MIN="$((10#${TIME##*:}))"
    mkdir -p "$HOME/Library/LaunchAgents"
    cat > "$PLIST" <<EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
  <key>Label</key><string>$LABEL</string>
  <key>ProgramArguments</key><array><string>/bin/bash</string><string>$SCRIPT</string></array>
  <key>StartCalendarInterval</key><dict><key>Hour</key><integer>$HOUR</integer><key>Minute</key><integer>$MIN</integer></dict>
  <key>StandardErrorPath</key><string>$HOME/.claude/backup.launchd.err</string>
</dict>
</plist>
EOF
    launchctl bootstrap "gui/$(id -u)" "$PLIST"
    echo "Scheduled daily at $TIME via $PLIST. Pause: $0 --remove. Log: ~/.claude/backup.log"
    ;;
  Linux)
    current="$(crontab -l 2>/dev/null | grep -v "$SCRIPT" || true)"
    if [ "${1:-}" = "--remove" ]; then printf '%s\n' "$current" | crontab -; echo "Removed cron entry."; exit 0; fi
    HOUR="$((10#${TIME%%:*}))"; MIN="$((10#${TIME##*:}))"
    printf '%s\n%s\n' "$current" "$MIN $HOUR * * * /bin/bash $SCRIPT" | sed '/^$/d' | crontab -
    echo "Scheduled daily at $TIME via crontab. Pause: $0 --remove. Log: ~/.claude/backup.log"
    ;;
  *)
    echo "Unsupported OS. On Windows, from Git Bash:"
    echo "  schtasks /Create /SC DAILY /ST $TIME /TN ClaudeBackup /TR \"\\\"C:\\Program Files\\Git\\bin\\bash.exe\\\" $SCRIPT\""
    exit 1
    ;;
esac
