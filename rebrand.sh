#!/usr/bin/env bash
# Rebrand script for a Flameshot fork.
# Run this from the ROOT of your cloned flameshot repo.
#
# Usage:
#   ./rebrand.sh MyShot myshot com.letmegrab.myshot
#
#   $1 = Display name, capitalized   (e.g. "MyShot")
#   $2 = Binary/lowercase name       (e.g. "myshot")
#   $3 = Reverse-DNS app id          (e.g. "com.letmegrab.myshot")

set -euo pipefail

NEW_NAME_CAP="${1:?Usage: $0 <DisplayName> <lowercase_name> <reverse.dns.id>}"
NEW_NAME_LOWER="${2:?}"
NEW_APPID="${3:?}"

OLD_NAME_CAP="Flameshot"
OLD_NAME_LOWER="flameshot"
OLD_APPID="org.flameshot.Flameshot"

echo ">> Renaming text occurrences (code, cmake, desktop/appdata/dbus xml, man, shell completions)..."
grep -rIl "${OLD_NAME_CAP}\|${OLD_NAME_LOWER}\|${OLD_APPID}" \
    --include=*.cpp --include=*.h --include=*.cmake --include=CMakeLists.txt \
    --include=*.desktop --include=*.xml --include=*.xml.in --include=*.service.in \
    --include=*.metainfo.xml --include=*.1 --include=*.zsh --include=*.fish \
    --include=*.bash --include=*.rc . 2>/dev/null | while read -r f; do
  sed -i \
    -e "s/${OLD_APPID}/${NEW_APPID}/g" \
    -e "s/${OLD_NAME_CAP}/${NEW_NAME_CAP}/g" \
    -e "s/${OLD_NAME_LOWER}/${NEW_NAME_LOWER}/g" \
    "$f"
done

echo ">> Renaming files/directories containing 'flameshot' in the name..."
# Deepest paths first so renaming a dir doesn't break child file paths
find . -depth -iname "*flameshot*" ! -path "./.git/*" | while read -r p; do
  newp=$(echo "$p" | sed \
    -e "s/${OLD_APPID}/${NEW_APPID}/g" \
    -e "s/${OLD_NAME_CAP}/${NEW_NAME_CAP}/g" \
    -e "s/${OLD_NAME_LOWER}/${NEW_NAME_LOWER}/g")
  if [ "$p" != "$newp" ]; then
    mkdir -p "$(dirname "$newp")"
    git mv "$p" "$newp" 2>/dev/null || mv "$p" "$newp"
  fi
done

echo ">> Done."
echo "Manual steps still required:"
echo "  1. Replace icon artwork under data/img/** with your own (same filenames after rename)."
echo "  2. Update data/appdata/${NEW_APPID}.metainfo.xml description/screenshots/author info."
echo "  3. Update CMakeLists.txt COPYRIGHT/AUTHOR strings and about-dialog credits"
echo "     (src/widgets/*aboutdialog* or similar) with your own attribution -- keep the"
echo "     GPLv3 license notice and upstream copyright, since it's legally required for a fork."
echo "  4. Register the new uploader: add localassets/ files to CMakeLists.txt src list,"
echo "     wire it into imguploadermanager.cpp/toolfactory.cpp in place of / alongside Imgur."
echo "  5. Rebuild: cmake -B build -S . && cmake --build build"
