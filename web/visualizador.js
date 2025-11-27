class ExecutionVisualizer {
    constructor() {
        this.traceData = [];
        this.currentStep = 0;
        this.autoInterval = null;
        this.speed = 500;
        this.stackBase = null;
        this.init();
    }

    async init() {
        this.setupEventListeners();
        this.setupFileInput();
        this.updateSpeedDisplay();
        this.showWelcomeMessage();
        
        // ✅ Intentar cargar automáticamente DESPUÉS de setup
        await this.loadTrace();
    }

    showWelcomeMessage() {
        const codeDisplay = document.getElementById('assembly-code');
        codeDisplay.textContent = `🎯 Visualizador de Ejecución de Compilador

📋 Cómo usar:
1. Genera la traza ejecutando tu programa compilado
2. Haz clic en "📂 Cargar Traza" y selecciona execution_trace.json
3. Usa los controles para navegar paso a paso

⌨️ Atajos de teclado:
   ← → : Navegar entre pasos
   Espacio: Play/Pause
   Home: Reiniciar

🔍 O coloca execution_trace.json en la carpeta web/`;
        codeDisplay.style.color = '#4ec9b0';
    }

    setupFileInput() {
        const controls = document.querySelector('.controls');
        
        const fileInputContainer = document.createElement('div');
        fileInputContainer.style.cssText = 'margin: 10px 0; text-align: center;';
        
        const fileInput = document.createElement('input');
        fileInput.type = 'file';
        fileInput.id = 'trace-file-input';
        fileInput.accept = '.json';
        fileInput.style.display = 'none';
        
        const fileButton = document.createElement('button');
        fileButton.textContent = '📂 Cargar Traza (JSON)';
        fileButton.className = 'btn';
        fileButton.style.cssText = 'background: #0e639c; padding: 12px 24px; font-size: 16px;';
        fileButton.onclick = () => fileInput.click();
        
        fileInput.addEventListener('change', (e) => this.handleFileSelect(e));
        
        fileInputContainer.appendChild(fileButton);
        fileInputContainer.appendChild(fileInput);
        controls.insertBefore(fileInputContainer, controls.firstChild);
    }

    async handleFileSelect(event) {
        const file = event.target.files[0];
        if (!file) return;

        try {
            console.log('📄 Cargando archivo:', file.name);
            const text = await file.text();
            
            this.traceData = JSON.parse(text.trim());
            
            console.log('✅ Traza cargada:', this.traceData.length, 'pasos');
            
            if (this.traceData.length === 0) {
                this.showError('El archivo de traza está vacío.');
                return;
            }
            
            // ✅ Ahora sí renderizar
            this.renderStep(0);
            
        } catch (error) {
            console.error('❌ Error cargando traza:', error);
            this.showError(`Error parseando JSON: ${error.message}\n\nVerifica que el archivo sea un JSON válido.`);
        }
    }

    async loadTrace() {
        try {
            console.log('🔄 Intentando cargar traza desde URL...');
            
            const possiblePaths = [
                'execution_trace.json',
                '../execution_trace.json',
                './execution_trace.json'
            ];
            
            for (const path of possiblePaths) {
                try {
                    const response = await fetch(path);
                    if (response.ok) {
                        const text = await response.text();
                        this.traceData = JSON.parse(text.trim());
                        console.log('✅ Traza cargada desde:', path, '-', this.traceData.length, 'pasos');
                        
                        if (this.traceData.length > 0) {
                            this.renderStep(0);
                            return;
                        }
                    }
                } catch (e) {
                    console.log('❌ No se pudo cargar desde:', path);
                    continue;
                }
            }
            
            console.log('ℹ️ No se encontró traza automáticamente');
            
        } catch (error) {
            console.log('ℹ️ Use el botón "Cargar Traza"');
        }
    }

    showError(message) {
        const codeDisplay = document.getElementById('assembly-code');
        codeDisplay.textContent = `❌ ERROR: ${message}\n\n💡 Cómo generar una traza:\n1. gcc programa.s runtime_trace.o -o programa\n2. ./programa\n3. Verifica: ls -lh execution_trace.json\n4. Carga el archivo con "📂 Cargar Traza"`;
        codeDisplay.style.color = '#f48771';
    }

    setupEventListeners() {
        document.getElementById('prev-step').addEventListener('click', () => this.prevStep());
        document.getElementById('next-step').addEventListener('click', () => this.nextStep());
        document.getElementById('auto-step').addEventListener('click', () => this.toggleAutoStep());
        document.getElementById('reset').addEventListener('click', () => this.reset());
        
        document.getElementById('speed').addEventListener('input', (e) => {
            this.speed = 1000 - (e.target.value * 90);
            this.updateSpeedDisplay();
            if (this.autoInterval) {
                this.toggleAutoStep();
                this.toggleAutoStep();
            }
        });
        
        document.addEventListener('keydown', (e) => {
            // ✅ Solo responder a teclas si hay datos cargados
            if (this.traceData.length === 0) return;
            
            switch(e.key) {
                case 'ArrowLeft':
                    e.preventDefault();
                    this.prevStep();
                    break;
                case 'ArrowRight':
                    e.preventDefault();
                    this.nextStep();
                    break;
                case ' ':
                    e.preventDefault();
                    this.toggleAutoStep();
                    break;
                case 'Home':
                    e.preventDefault();
                    this.reset();
                    break;
            }
        });
    }

    updateSpeedDisplay() {
        const speedValue = document.getElementById('speed-value');
        const speedInput = document.getElementById('speed');
        if (speedValue && speedInput) {
            speedValue.textContent = speedInput.value;
        }
    }

    renderStep(step) {
        // ✅ VALIDACIONES ROBUSTAS
        if (!this.traceData || this.traceData.length === 0) {
            console.log('⚠️ No hay datos de traza cargados');
            return;
        }
        
        if (step < 0 || step >= this.traceData.length) {
            console.log('⚠️ Paso fuera de rango:', step);
            return;
        }
        
        this.currentStep = step;
        const state = this.traceData[step];
        
        // ✅ Validar que el estado tenga la estructura correcta
        if (!state || !state.registers) {
            console.error('❌ Estado inválido en paso:', step, state);
            this.showError(`Paso ${step} tiene datos inválidos`);
            return;
        }
        
        this.updateRegisters(state.registers);
        this.updateCodeDisplay(state);
        this.updateStackDisplay(state.registers);
        this.updateStepInfo();
        this.highlightChanges();
    }

    updateRegisters(registers) {
        // ✅ Validación adicional
        if (!registers || typeof registers !== 'object') {
            console.error('❌ Registros inválidos:', registers);
            return;
        }
        
        const container = document.getElementById('registers-container');
        if (!container) return;
        
        for (const [name, value] of Object.entries(registers)) {
            const registerElement = Array.from(container.children).find(
                child => {
                    const nameEl = child.querySelector('.register-name');
                    return nameEl && nameEl.textContent === name.toUpperCase();
                }
            );
            
            if (registerElement) {
                const valueElement = registerElement.querySelector('.register-value');
                if (valueElement) {
                    const previousValue = valueElement.textContent;
                    valueElement.textContent = value;
                    
                    if (previousValue !== value && previousValue !== '0x0000000000000000') {
                        registerElement.classList.add('highlight');
                        setTimeout(() => registerElement.classList.remove('highlight'), 1500);
                    }
                }
            }
        }
    }

    updateCodeDisplay(state) {
        const instrEl = document.getElementById('current-instruction');
        const sourceEl = document.getElementById('source-context');
        const lineEl = document.getElementById('line-number');
        const codeEl = document.getElementById('assembly-code');
        
        if (instrEl) instrEl.textContent = state.instruction || 'N/A';
        if (sourceEl) sourceEl.textContent = state.source || 'N/A';
        if (lineEl) lineEl.textContent = state.line || 'N/A';
        
        if (codeEl) {
            const asmCode = this.generateAssemblyCode(state);
            codeEl.textContent = asmCode;
            codeEl.style.color = '#d7ba7d';
        }
    }

    generateAssemblyCode(state) {
        let code = `# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n`;
        code += `# Paso ${state.step + 1}/${this.traceData.length}\n`;
        code += `# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n`;
        code += `# Instrucción: ${state.instruction || 'N/A'}\n`;
        code += `# Contexto: ${state.source || 'N/A'}\n`;
        code += `# Línea: ${state.line || 'N/A'}\n\n`;
        code += `# Estado de registros:\n`;
        
        if (state.registers) {
            code += `movq  ${state.registers.rax}, %rax\n`;
            code += `movq  ${state.registers.rbx}, %rbx\n`;
            code += `movq  ${state.registers.rcx}, %rcx\n`;
            code += `movq  ${state.registers.rdx}, %rdx\n`;
            code += `movq  ${state.registers.rsi}, %rsi\n`;
            code += `movq  ${state.registers.rdi}, %rdi\n`;
            code += `movq  ${state.registers.rbp}, %rbp\n`;
            code += `movq  ${state.registers.rsp}, %rsp\n`;
        }
        
        return code;
    }

    updateStackDisplay(registers) {
        const memoryDisplay = document.getElementById('memory-display');
        const rspValue = document.getElementById('rsp-value');
        
        if (!registers || !registers.rsp) return;
        if (rspValue) rspValue.textContent = registers.rsp;
        
        if (memoryDisplay) {
            const currentRsp = BigInt(registers.rsp);
            
            // Fijar la BASE al inicio (Trace paso 0)
            if (this.traceData.length > 0 && (!this.stackBase || this.currentStep === 0)) {
                this.stackBase = BigInt(this.traceData[0].registers.rsp);
            }
            
            const base = this.stackBase || currentRsp;
            let memoryHTML = '';
            
            // -----------------------------------------------------------
            // CAMBIO CLAVE: De i < 7 a i < 20 (Para ver más profundidad)
            // Empezamos en -2 para ver un poco "arriba" de la base
            // -----------------------------------------------------------
            for (let i = -2; i < 20; i++) {
                // Calculamos la dirección de esta celda fija
                const cellAddr = base - BigInt(i * 8);
                const isCurrentRsp = (cellAddr === currentRsp);
                
                // Etiquetas para entender dónde estamos
                let label = "";
                let offset = i * 8;
                
                if (i === 0) label = "BASE (Inicio)";
                else if (i < 0) label = `[ BASE + ${Math.abs(offset)} ]`; 
                else label = `[ BASE - ${offset} ]`; 

                // Intentar identificar tus variables por su offset típico
                // (Ajustado a tu código: RBP suele ser Base-8 tras el primer push)
                let extraInfo = "";
                if (offset === 16) extraInfo = " (Var -8)";
                if (offset === 24) extraInfo = " (Var -16)";
                if (offset === 32) extraInfo = " (Var -24)";
                if (offset === 72) extraInfo = " (Meta del subq $64)";

                const addrHex = '0x' + cellAddr.toString(16);

                memoryHTML += `<div class="memory-cell ${isCurrentRsp ? 'highlight' : ''}" title="${addrHex}">
                    <div style="font-size:0.75em; color:#aaa; margin-bottom:2px;">
                        ${label} <span style="color:#4ec9b0">${extraInfo}</span>
                    </div>
                    <div style="font-weight:bold; color: ${isCurrentRsp ? '#fff' : '#555'}">
                        ${isCurrentRsp ? '📍 RSP AQUÍ' : '...'}
                    </div>
                </div>`;
            }
            
            memoryDisplay.innerHTML = memoryHTML;
        }
    }

    updateStepInfo() {
        const stepInfo = document.getElementById('step-info');
        if (stepInfo) {
            stepInfo.textContent = `Paso: ${this.currentStep + 1}/${this.traceData.length}`;
        }
        
        const prevBtn = document.getElementById('prev-step');
        const nextBtn = document.getElementById('next-step');
        
        if (prevBtn) prevBtn.disabled = this.currentStep === 0;
        if (nextBtn) nextBtn.disabled = this.currentStep === this.traceData.length - 1;
    }

    highlightChanges() {
        const panels = document.querySelectorAll('.panel');
        panels.forEach(panel => {
            panel.style.boxShadow = '0 0 20px rgba(0, 122, 204, 0.5)';
            setTimeout(() => {
                panel.style.boxShadow = '0 4px 12px rgba(0, 0, 0, 0.2)';
            }, 500);
        });
    }

    nextStep() {
        if (this.traceData.length === 0) return;
        if (this.currentStep < this.traceData.length - 1) {
            this.renderStep(this.currentStep + 1);
        }
    }

    prevStep() {
        if (this.traceData.length === 0) return;
        if (this.currentStep > 0) {
            this.renderStep(this.currentStep - 1);
        }
    }

    toggleAutoStep() {
        if (this.traceData.length === 0) return;
        
        const button = document.getElementById('auto-step');
        if (!button) return;
        
        if (this.autoInterval) {
            clearInterval(this.autoInterval);
            this.autoInterval = null;
            button.textContent = '▶️ Auto-ejecutar';
            button.style.background = '#0e639c';
        } else {
            this.autoInterval = setInterval(() => {
                if (this.currentStep < this.traceData.length - 1) {
                    this.nextStep();
                } else {
                    this.toggleAutoStep();
                }
            }, this.speed);
            button.textContent = '⏸️ Pausar';
            button.style.background = '#d16969';
        }
    }

    reset() {
        if (this.autoInterval) {
            this.toggleAutoStep();
        }
        this.stackBase = null;
        if (this.traceData.length > 0) {
            this.renderStep(0);
        }
    }
}

document.addEventListener('DOMContentLoaded', () => {
    console.log('🎮 Inicializando visualizador...');
    const visualizer = new ExecutionVisualizer();
    window.visualizer = visualizer;
    
    console.log('🎮 Controles:');
    console.log('  ← →: Navegar');
    console.log('  Espacio: Play/Pause');
    console.log('  Home: Reiniciar');
});