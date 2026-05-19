# Dirichlet-Problem
Numeric methods assignment on solving Dirichlet  problem for Poisson's equation using

---
## Инструкция по сборке

*Используемая версия Python:* **3.14**.

На Linux необходимо установить `dev` версию python:
```Shell
sudo apt install python3.14-dev
```

#### 1. Создать виртуальное окружение
```Shell
py -m venv .venv
```

*Команда `py` может не работать на Linux.*
#### 2. Активировать виртуальное окружение
```Shell
./.venv/Scripts/activate
```
#### 3. Установить библиотеки
```Shell
pip install -r "requirements.txt"
```
#### 4. Папка для сборки
```bash
mkdir build
cd build
```
#### 5. Генерация файлов сборки
```bash
cmake .. 
```
#### 6. Компиляция программы
```bash
cmake --build .
```

--- 
## Запуск

Для запуска введите следующую команду:
```Shell
streamlit run .\python\main.py
```

---
### Другое
#### Intellisense для Vs Code
Добавьте деректорию `./python` как дополнительный путь в `python.analysis.extraPaths`
```json
"python.analysis.extraPaths": ["${workspaceFolder}/python"]
```