# CI Pipeline Usage Guide

## 🎯 **Pipeline Features**

### **Smart Triggering**
- **Manual Run**: Click "Run workflow" in GitHub Actions tab
- **Pull Requests**: Automatically runs on PRs to `main`
- **Releases**: Runs on pushes to `main` branch
- **Path Filtering**: Skips runs for documentation-only changes
- **Auto-Cancel**: Stops previous runs when new commits are pushed

### **Concurrency Control** ⚡
- **Smart Cancellation**: New commits automatically cancel older pipeline runs
- **Per-Branch**: Each branch/PR has independent pipeline runs  
- **Resource Saving**: Prevents multiple runs of the same branch running simultaneously
- **Latest Always Wins**: Only the most recent commit gets validated

### **Timeout Safeguards**
- **Native Tests**: 10 minutes maximum
- **ESP32 Build**: 15 minutes maximum  
- **Validation**: 2 minutes maximum
- **Fallback**: 20 minutes maximum

### **Multi-Runner Support**
- **Default**: Ubuntu (fastest, cheapest)
- **Manual**: Choose Ubuntu, macOS, or Windows
- **Fallback**: Tries all runners if manual run fails

## 🚀 **How to Use**

### **1. Manual Pipeline Run**
```bash
# Go to GitHub → Actions → CI Pipeline → Run workflow
# Choose your preferred runner environment
# Click "Run workflow"
```

### **2. Automatic on Pull Request**
```bash
# The pipeline runs automatically when you:
git checkout -b feature-branch
# make your changes
git commit -m "Add new feature"
git push origin feature-branch
# Create PR to main → Pipeline runs automatically
```

### **3. Skip Pipeline for Docs**
```bash
# These file changes WON'T trigger the pipeline:
docs/
README.md
*.md files
.gitignore
LICENSE
.vscode/
.scripts/README.md
```

## 🛡️ **Safeguards in Action**

### **Concurrency Protection**
When you push multiple commits quickly:
```bash
git commit -m "Fix bug"
git push                    # Pipeline starts

git commit -m "Another fix" 
git push                    # Cancels previous run, starts new one
```
- ✅ Only latest commit gets tested
- ✅ Saves CI minutes from cancelled runs
- ✅ Per-branch isolation (PR1 doesn't cancel PR2)

### **Timeout Protection**
If a job hangs or takes too long:
- Native tests timeout after 10 minutes
- ESP32 build timeout after 15 minutes
- No more infinite waiting!

### **Runner Fallback** (Manual runs only)
If the primary runner fails:
1. Pipeline tries Ubuntu first
2. If manual run fails → Fallback tries all OS types
3. Helps identify OS-specific issues

### **Smart Path Filtering**
Pipeline skips when you only change:
- Documentation files
- README updates
- License changes  
- VS Code settings

## 📊 **Pipeline Results**

### **Success Output**
```
🎉 All validations passed!
✅ Native tests: PASSED
✅ ESP32 build: PASSED

Ready for deployment! 🚀
```

### **Failure Output**
```
💥 Pipeline failed!
❌ Native tests: FAILED
❌ ESP32 build: SUCCESS

💡 Manual run detected - consider trying a different runner if this was a timeout/runner issue
```

## ⚡ **Quick Commands**

### **Local Testing** (before pushing)
```bash
# Test locally first to save CI minutes
./.scripts/test.sh          # Run native tests
./.scripts/build.sh         # Build ESP32
./.scripts/ci-local.sh      # Simulate full pipeline
```

### **Manual Pipeline Triggers**
```bash
# When you need to run the pipeline manually:
# 1. Go to GitHub → Your Repo → Actions tab
# 2. Select "CI Pipeline" workflow
# 3. Click "Run workflow" button
# 4. Choose runner (Ubuntu recommended)
# 5. Click green "Run workflow" button
```

## 🎛️ **Advanced Usage**

### **Runner Selection Strategy**
- **Ubuntu**: Fastest, cheapest, use for most development
- **macOS**: Test macOS compatibility, uses 10x minutes
- **Windows**: Test Windows compatibility, uses 2x minutes

### **Troubleshooting Timeouts**
If you hit timeouts:
1. Check if your code has infinite loops
2. Try a different runner (macOS has more resources)
3. Optimize your build (remove debug flags)
4. Split large changes into smaller PRs

### **Monitoring Usage**
```bash
# Check your GitHub Actions usage:
# GitHub → Settings → Billing → Plans and usage
# Watch for approaching the 2000 minute limit
```

## 📈 **Best Practices**

1. **Use Local Testing**: Run `./.scripts/ci-local.sh` before pushing
2. **Manual Runs**: Use for experimental branches or troubleshooting
3. **Path Filtering**: Separate doc updates from code changes  
4. **Runner Choice**: Stick with Ubuntu unless testing OS compatibility
5. **Timeout Awareness**: Keep builds under 10 minutes when possible
6. **Quick Iterations**: Push freely - old pipelines auto-cancel to save resources

## ⚡ **Concurrency Benefits**

### **Development Workflow**
```bash
# Scenario: You're iterating quickly on a feature
git commit -m "Initial implementation"
git push origin feature-branch        # Pipeline #1 starts

# You notice a bug and fix it immediately  
git commit -m "Fix compilation error"
git push origin feature-branch        # Pipeline #1 cancelled, #2 starts

# Another quick fix
git commit -m "Fix test failure" 
git push origin feature-branch        # Pipeline #2 cancelled, #3 starts

# Result: Only Pipeline #3 runs to completion
# Savings: ~20 minutes of CI time instead of 60 minutes!
```

### **Team Collaboration**
- **Branch Isolation**: Your PR pipeline won't cancel teammate's PR pipeline
- **Main Branch Protection**: Pushes to main still run independently
- **Manual Runs**: Manual triggers are isolated from automatic runs

This pipeline is designed to be efficient, safe, and flexible for your ESP32 development workflow! 🏎️