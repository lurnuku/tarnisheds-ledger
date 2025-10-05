#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <WebServer.h>

const char HTML_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="icon" href="data:image/svg+xml,<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 100 100'><circle cx='50' cy='50' r='45' fill='%23d4c5a0'/><circle cx='50' cy='50' r='35' fill='none' stroke='%23120f0c' stroke-width='3'/><text x='50' y='70' text-anchor='middle' font-size='50' fill='%23120f0c'>✦</text></svg>">
    <title>The Tarnished's Ledger</title>
    <style>
        @import url('https://fonts.googleapis.com/css2?family=Spectral:wght@300;400;500&display=swap');
        
        * { margin: 0; padding: 0; box-sizing: border-box; }
        
        body { 
            font-family: 'Spectral', serif;
            background: linear-gradient(to bottom, rgba(18, 15, 12, 1) 0%, rgba(12, 10, 8, 1) 100%);
            min-height: 100vh;
            color: #c9b896;
            position: relative;
            overflow: scroll;
        }
        
        body::before {
            content: '';
            position: fixed;
            top: 0;
            left: 0;
            width: 100%;
            height: 100%;
            background: 
                radial-gradient(ellipse at 50% 100%, rgba(40, 35, 25, 0.3) 0%, transparent 60%),
                radial-gradient(ellipse at 50% 0%, rgba(20, 18, 15, 0.5) 0%, transparent 50%);
            pointer-events: none;
        }
        
        .page-header {
            display: flex;
            align-items: center;
            padding: 18px 36px;
            gap: 12px;
            border-bottom: 1px solid rgba(201, 184, 150, 0.15);
        }
        
        .header-icon {
            width: 38px;
            height: 38px;
            background: rgba(80, 70, 55, 0.4);
            border: 2px solid rgba(201, 184, 150, 0.3);
            display: flex;
            align-items: center;
            justify-content: center;
            font-size: 20px;
        }
        
        .header-title {
            font-size: 22px;
            font-weight: 300;
            color: #c9b896;
            letter-spacing: 1px;
        }
        
        .main-content {
            padding: 30px 36px;
            max-width: 1200px;
            margin: 0 auto;
            position: relative;
        }
        
        .section-title {
            font-size: 17px;
            font-weight: 300;
            color: #c9b896;
            margin-bottom: 20px;
            padding-bottom: 10px;
            border-bottom: 1px solid rgba(201, 184, 150, 0.2);
            letter-spacing: 1px;
        }
        
        .stats-container {
            display: flex;
            gap: 50px;
            margin-bottom: 35px;
            padding: 22px 25px;
            background: rgba(0, 0, 0, 0.2);
            border: 1px solid rgba(201, 184, 150, 0.15);
        }
        
        .stat-item {
            flex: 1;
            text-align: center;
        }
        
        .stat-label {
            font-size: 14px;
            color: rgba(201, 184, 150, 0.6);
            margin-bottom: 8px;
            letter-spacing: 0.5px;
            font-weight: 300;
        }
        
        .stat-value {
            font-size: 42px;
            color: #d4c5a0;
            font-weight: 300;
            line-height: 1;
        }
        
        .controls-section {
            background: rgba(0, 0, 0, 0.15);
            border: 1px solid rgba(201, 184, 150, 0.15);
            padding: 25px;
            margin-bottom: 30px;
        }
        
        .control-row {
            display: flex;
            align-items: center;
            gap: 20px;
            margin-bottom: 0;
        }
        
        .control-row:last-child {
            margin-bottom: 0;
        }
        
        .control-label {
            font-size: 18px;
            color: #c9b896;
            min-width: 180px;
            font-weight: 300;
        }
        
        .control-input {
            flex: 1;
            background: rgba(0, 0, 0, 0.4);
            border: 1px solid rgba(201, 184, 150, 0.25);
            color: #c9b896;
            padding: 14px 20px;
            font-family: 'Spectral', serif;
            font-size: 17px;
            transition: all 0.2s;
        }
        
        .control-input:focus {
            outline: none;
            border-color: rgba(201, 184, 150, 0.5);
            background: rgba(0, 0, 0, 0.5);
        }
        
        .control-input::placeholder {
            color: rgba(201, 184, 150, 0.3);
        }
        
        .control-button {
            background: rgba(40, 35, 28, 0.6);
            border: 1px solid rgba(201, 184, 150, 0.3);
            color: #c9b896;
            padding: 14px 32px;
            font-family: 'Spectral', serif;
            font-size: 16px;
            cursor: pointer;
            transition: all 0.2s;
            letter-spacing: 0.5px;
        }
        
        .control-button:hover {
            background: rgba(50, 45, 38, 0.8);
            border-color: rgba(201, 184, 150, 0.5);
        }
        
        .items-list {
            list-style: none;
        }
        
        .list-item {
            background: rgba(0, 0, 0, 0.15);
            border-left: 3px solid rgba(201, 184, 150, 0.3);
            padding: 20px 25px;
            margin-bottom: 2px;
            display: flex;
            align-items: center;
            gap: 20px;
            transition: all 0.15s;
            cursor: pointer;
            position: relative;
        }
        
        .list-item::before {
            content: '';
            position: absolute;
            left: 0;
            top: 0;
            width: 0;
            height: 100%;
            background: rgba(201, 184, 150, 0.05);
            transition: width 0.15s;
        }
        
        .list-item:hover::before {
            width: 100%;
        }
        
        .list-item:hover {
            background: rgba(0, 0, 0, 0.25);
            border-left-color: rgba(201, 184, 150, 0.5);
        }
        
        .list-item.completed {
            opacity: 0.5;
            border-left-color: rgba(201, 184, 150, 0.15);
        }
        
        .list-item.completed .item-text {
            text-decoration: line-through;
            color: rgba(201, 184, 150, 0.4);
        }
        
        .item-checkbox {
            display: none;
        }
        
        .custom-checkbox {
            width: 26px;
            height: 26px;
            border: 2px solid rgba(201, 184, 150, 0.4);
            background: rgba(0, 0, 0, 0.4);
            cursor: pointer;
            position: relative;
            z-index: 2;
            transition: all 0.2s;
            display: flex;
            align-items: center;
            justify-content: center;
            flex-shrink: 0;
        }
        
        .custom-checkbox::before {
            content: '';
            position: absolute;
            inset: 0;
            background: linear-gradient(135deg, rgba(201, 184, 150, 0.05) 0%, transparent 100%);
        }
        
        .custom-checkbox:hover {
            border-color: rgba(201, 184, 150, 0.6);
            background: rgba(0, 0, 0, 0.5);
            box-shadow: 0 0 10px rgba(201, 184, 150, 0.2);
        }
        
        .custom-checkbox-mark {
            opacity: 0;
            transform: scale(0) rotate(-45deg);
            transition: all 0.3s cubic-bezier(0.68, -0.55, 0.265, 1.55);
            color: #d4c5a0;
            font-size: 18px;
            line-height: 1;
            text-shadow: 0 0 8px rgba(212, 197, 160, 0.6);
        }
        
        .item-checkbox:checked + .custom-checkbox {
            border-color: rgba(201, 184, 150, 0.7);
            background: rgba(40, 35, 28, 0.6);
            box-shadow: 
                inset 0 0 10px rgba(201, 184, 150, 0.2),
                0 0 15px rgba(201, 184, 150, 0.3);
        }
        
        .item-checkbox:checked + .custom-checkbox .custom-checkbox-mark {
            opacity: 1;
            transform: scale(1) rotate(0deg);
        }
        
        .item-text {
            flex: 1;
            font-size: 18px;
            color: #c9b896;
            font-weight: 300;
            position: relative;
            z-index: 2;
        }
        
        .item-button {
            background: rgba(60, 30, 30, 0.4);
            border: 1px solid rgba(150, 80, 80, 0.4);
            color: rgba(201, 150, 150, 0.8);
            padding: 10px 16px;
            font-family: 'Spectral', serif;
            font-size: 14px;
            cursor: pointer;
            transition: all 0.2s;
            position: relative;
            z-index: 2;
        }
        
        .item-button:hover {
            background: rgba(80, 40, 40, 0.6);
            border-color: rgba(180, 100, 100, 0.6);
            color: rgba(220, 180, 180, 0.9);
        }
        
        .danger-button {
            width: 100%;
            background: rgba(60, 30, 30, 0.3);
            border: 1px solid rgba(150, 80, 80, 0.4);
            color: rgba(201, 150, 150, 0.8);
            padding: 18px;
            font-family: 'Spectral', serif;
            font-size: 17px;
            cursor: pointer;
            transition: all 0.2s;
            margin-top: 30px;
            letter-spacing: 0.5px;
        }
        
        .danger-button:hover {
            background: rgba(80, 40, 40, 0.5);
            border-color: rgba(180, 100, 100, 0.6);
            color: rgba(220, 180, 180, 0.9);
        }
        
        .empty-state {
            text-align: center;
            padding: 80px 40px;
            color: rgba(201, 184, 150, 0.4);
            font-size: 18px;
            font-weight: 300;
            line-height: 1.8;
        }
        
        .loading-banner {
            position: fixed;
            top: 0;
            left: 0;
            right: 0;
            background: linear-gradient(135deg, rgba(40, 35, 28, 0.95) 0%, rgba(30, 25, 20, 0.95) 100%);
            border-bottom: 2px solid rgba(201, 184, 150, 0.4);
            padding: 20px 40px;
            display: none;
            align-items: center;
            gap: 20px;
            z-index: 9999;
            box-shadow: 0 4px 20px rgba(0, 0, 0, 0.5);
        }
        
        .loading-banner.active {
            display: flex;
            animation: slideDown 0.3s ease-out;
        }
        
        @keyframes slideDown {
            from {
                transform: translateY(-100%);
                opacity: 0;
            }
            to {
                transform: translateY(0);
                opacity: 1;
            }
        }
        
        .loading-icon {
            width: 32px;
            height: 32px;
            position: relative;
        }
        
        .loading-icon::before {
            content: '◈';
            position: absolute;
            top: 50%;
            left: 50%;
            transform: translate(-50%, -50%);
            font-size: 28px;
            color: #d4c5a0;
            animation: iconSpin 2s linear infinite;
        }
        
        @keyframes iconSpin {
            0% { transform: translate(-50%, -50%) rotate(0deg); }
            100% { transform: translate(-50%, -50%) rotate(360deg); }
        }
        
        .loading-text {
            font-size: 16px;
            color: #c9b896;
            letter-spacing: 1px;
            font-weight: 300;
        }
        
        .loading-dots {
            display: inline-block;
        }
        
        .loading-dots::after {
            content: '';
            animation: dots 1.5s steps(4, end) infinite;
        }
        
        @keyframes dots {
            0%, 20% { content: ''; }
            40% { content: '.'; }
            60% { content: '..'; }
            80%, 100% { content: '...'; }
        }
        
        .loading-progress {
            flex: 1;
            height: 4px;
            background: rgba(0, 0, 0, 0.4);
            border: 1px solid rgba(201, 184, 150, 0.2);
            position: relative;
            overflow: hidden;
        }
        
        .loading-progress-bar {
            position: absolute;
            top: 0;
            left: 0;
            height: 100%;
            background: linear-gradient(90deg, 
                rgba(201, 184, 150, 0.3) 0%, 
                rgba(212, 197, 160, 0.6) 50%, 
                rgba(201, 184, 150, 0.3) 100%);
            width: 40%;
            animation: progressSlide 1.5s ease-in-out infinite;
        }
        
        @keyframes progressSlide {
            0% { left: -40%; }
            100% { left: 100%; }
        }

        .char-counter {
            text-align: right;
            font-size: 13px;
            color: rgba(201, 184, 150, 0.5);
            margin-top: 5px;
            margin-right: 10px;
            font-weight: 300;
        }
    </style>
</head>
<body>
    <div class="loading-banner" id="loadingBanner">
        <div class="loading-icon"></div>
        <div class="loading-text">
            Updating e-ink display<span class="loading-dots"></span>
        </div>
        <div class="loading-progress">
            <div class="loading-progress-bar"></div>
        </div>
    </div>
    <div class="page-header">
        <div class="header-icon">☰</div>
        <div class="header-title">Endless Burdens Of The Tarnished</div>
    </div>
    <div class="main-content">
        <div class="section-title">Summary</div>
        <div class="stats-container">
            <div class="stat-item">
                <div class="stat-label">Total burdens</div>
                <div class="stat-value" id="totalCount">0</div>
            </div>
            <div class="stat-item">
                <div class="stat-label">Burdens yet unlifted</div>
                <div class="stat-value" id="activeCount">0</div>
            </div>
            <div class="stat-item">
                <div class="stat-label">Burdens set down</div>
                <div class="stat-value" id="completedCount">0</div>
            </div>
        </div>
        <div class="section-title">Take up a burden</div>
        <div class="controls-section">
            <div class="control-row">
                <div class="control-label">Deed to be done</div>
                <input type="text" maxlength="30" class="control-input" id="todoInput" placeholder="Inscribe the task..." oninput="updateCharCount()" onkeypress="if(event.key==='Enter') addTodo()">
                <button class="control-button" onclick="addTodo()">Undertake</button>
            </div>
            <div class="char-counter" id="charCounter">0 / 30</div>
        </div>
        <div class="section-title">Burdens that weigh upon thee</div>
        <ul class="items-list" id="todoList"></ul>
        <button class="danger-button" onclick="deleteAll()" id="deleteAllBtn" style="display:none;">Lay all burdens to rest</button>
    </div>
    <script>
    function showLoading() {
        document.getElementById('loadingBanner').classList.add('active')
    }
    
    function hideLoading() {
        document.getElementById('loadingBanner').classList.remove('active')
    }
    
    function loadTodos() {
        fetch('/todos')
            .then(function(r) { return r.json() })
            .then(function(data) {
                var list = document.getElementById('todoList')
                list.innerHTML = ''
                var todos = data.todos || []
                
                var total = todos.length
                var completed = 0
                
                if (todos.length === 0) {
                    list.innerHTML = '<div class="empty-state">The ledger lies empty</div>';
                    document.getElementById('deleteAllBtn').style.display = 'none'
                } else {
                    document.getElementById('deleteAllBtn').style.display = 'block'
                    for (var i = 0; i < todos.length; i++) {
                        var todo = todos[i]
                        if (todo.completed) completed++
                        var li = document.createElement('li')
                        li.className = 'list-item' + (todo.completed ? ' completed' : '')

                        // --- START OF FIX ---
                        var checkboxId = 'check' + i;
                        var listItemClickHandler = (function(id) {
                            return function(e) {
                                // Only trigger the checkbox if the clicked element is NOT the delete button.
                                // This is crucial because clicking the li triggers this, 
                                // but we don't want it to also toggle the todo if we are deleting it.
                                if (e.target.tagName !== 'BUTTON') {
                                    document.getElementById(id).click();
                                }
                            }
                        })(checkboxId);

                        li.onclick = listItemClickHandler;
                        // --- END OF FIX ---
                        
                        // The checkbox's native onchange handler will now correctly call toggleTodo(i)
                        li.innerHTML = '<input type="checkbox" class="item-checkbox" id="' + checkboxId + '" ' + (todo.completed ? 'checked' : '') + ' onchange="toggleTodo(' + i + ')"><label for="' + checkboxId + '" class="custom-checkbox"><span class="custom-checkbox-mark">✦</span></label><span class="item-text">' + escapeHtml(todo.text) + '</span><button class="item-button" onclick="deleteTodo(' + i + ')">X</button>'
                        list.appendChild(li)
                    }
                }
                
                document.getElementById('totalCount').textContent = total
                document.getElementById('activeCount').textContent = total - completed
                document.getElementById('completedCount').textContent = completed
            });
    }
    
    function addTodo() {
        var input = document.getElementById('todoInput')
        var text = input.value.trim()
        if (!text) return

        showLoading()
        
        fetch('/add', {
            method: 'POST',
            headers: {'Content-Type': 'application/x-www-form-urlencoded'},
            body: 'text=' + encodeURIComponent(text)
        }).then(function() {
            input.value = ''
            loadTodos()
            updateCharCount() // Update count after clearing input
            setTimeout(hideLoading, 1500)
        })
    }
    
    function toggleTodo(idx) {
        showLoading()
        fetch('/toggle?id=' + idx).then(function() { 
            loadTodos()
            setTimeout(hideLoading, 1500)
        })
    }
    
    function deleteTodo(idx) {
        showLoading()
        fetch('/delete?id=' + idx).then(function() { 
            loadTodos()
            setTimeout(hideLoading, 1500)
        })
    }
    
    function deleteAll() {
        showLoading()
        fetch('/deleteall').then(function() { 
            loadTodos()
            setTimeout(hideLoading, 1500)
        })
    }

    function updateCharCount() {
        var input = document.getElementById('todoInput')
        var counter = document.getElementById('charCounter')
        counter.textContent = input.value.length + ' / 30'
        
        if (input.value.length >= 30) {
            counter.style.color = 'rgba(201, 150, 150, 0.9)'
        } else {
            counter.style.color = 'rgba(201, 184, 150, 0.5)'
        }
    }
    
    function escapeHtml(text) {
        var div = document.createElement('div')
        div.textContent = text
        return div.innerHTML
    }
    
    loadTodos()
    setInterval(loadTodos, 5000)
</script>
</body>
</html>
)rawliteral";

#endif