# Usefull Commands

## 1. Pushing code to Remote / Origin / This repo

`git add <dateilocation>`
`git commit -m "<Type of Work> : <was man gemacht hat>"`
`git push`

---

## 2. Branch wechseln

`git fetch` → Holt die aktuellen Repo Infos
`git switch <branch Name>`

---

## 3. Neuen Branch erstellen

`git switch -c <branch Name>` → Erstellt und wechselt direkt
oder
`git branch <branch Name>`
`git switch <branch Name>`

---

## 4. Aktuellen Status prüfen

`git status` → Zeigt Änderungen & Staging Bereich
`git log` → Zeigt Commit Historie
`git log --oneline --graph --all` → Übersichtliche History

---

## 5. Änderungen anschauen

`git diff` → Nicht gestagte Änderungen
`git diff --staged` → Gestagte Änderungen

---

## 6. Änderungen rückgängig machen

`git restore <datei>` → Lokale Änderungen verwerfen
`git restore --staged <datei>` → Aus Staging entfernen
`git reset --hard` → Alles auf letzten Commit zurücksetzen ⚠️

---

## 7. Branches verwalten

`git branch` → Alle lokalen Branches anzeigen
`git branch -d <branch Name>` → Branch löschen
`git push origin --delete <branch Name>` → Remote Branch löschen

---

## 8. Updates vom Remote holen

`git pull` → Fetch + Merge
`git pull --rebase` → Fetch + Rebase

---

Wenn du willst, kann ich dir auch eine Version mit typischen Workflows (Feature Branch Flow, Hotfix Flow etc.) ergänzen.
