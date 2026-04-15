
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import numpy as np

# RUN:      python3 /workspaces/HeartController/scripts/plot_starling.py

# Starling curve points (from C++ code)
rv_points = [(0,0), (2,40), (6,60), (12,70)]
lv_points = [(0,0), (5,55), (12,85), (20,100)]

def evaluate(preload, points):
    x, y = zip(*points)
    if preload <= x[0]: return y[0]
    if preload >= x[-1]: return y[-1]
    for i in range(len(x) - 1):
        if x[i] <= preload <= x[i+1]:
            t = (preload - x[i]) / (x[i+1] - x[i])
            return y[i] + t * (y[i+1] - y[i])
    return 0

# Preload range
preload_range = np.linspace(0, 25, 100)
hr_values = [60, 72, 90, 120, 150, 180]
colors = plt.cm.viridis(np.linspace(0, 1, len(hr_values)))

# ========== SKRIV UT VÄRDEN ==========
print("\n" + "="*100)
print("STARLING CURVE VALUES - PRELOAD TO STROKE VOLUME")
print("="*100)

print("\nRIGHT VENTRICLE (RV):")
print("-"*50)
print(f"{'RAP (mmHg)':<12} {'SV (ml)':<10}")
print("-"*50)
for rap in [0, 2, 4, 6, 8, 10, 12, 15, 20]:
    sv = evaluate(rap, rv_points)
    print(f"{rap:<12.1f} {sv:<10.1f}")

print("\nLEFT VENTRICLE (LV):")
print("-"*50)
print(f"{'LAP (mmHg)':<12} {'SV (ml)':<10}")
print("-"*50)
for lap in [0, 3, 5, 8, 10, 12, 15, 18, 20, 25]:
    sv = evaluate(lap, lv_points)
    print(f"{lap:<12.1f} {sv:<10.1f}")

print("\n" + "="*100)
print("CARDIAC OUTPUT (L/min) = SV × HR / 1000")
print("="*100)

# Tabell för RV
print("\nRIGHT VENTRICLE - CO at different HR:")
print("-"*80)
header = f"{'RAP':<6}"
for hr in hr_values:
    header += f"HR={hr:<4} "
print(header)
print("-"*80)
for rap in [2, 4, 6, 8, 10, 12, 15]:
    sv = evaluate(rap, rv_points)
    row = f"{rap:<6.1f}"
    for hr in hr_values:
        co = (sv * hr) / 1000.0
        row += f"{co:<8.2f}"
    print(row)

# Tabell för LV
print("\nLEFT VENTRICLE - CO at different HR:")
print("-"*80)
header = f"{'LAP':<6}"
for hr in hr_values:
    header += f"HR={hr:<4} "
print(header)
print("-"*80)
for lap in [3, 5, 8, 10, 12, 15, 18, 20]:
    sv = evaluate(lap, lv_points)
    row = f"{lap:<6.1f}"
    for hr in hr_values:
        co = (sv * hr) / 1000.0
        row += f"{co:<8.2f}"
    print(row)

print("\n" + "="*100)
print("NORMAL VALUES COMPARISON")
print("="*100)
rap_normal = 6.0
lap_normal = 10.0
hr_normal = 72.0

sv_rv_normal = evaluate(rap_normal, rv_points)
sv_lv_normal = evaluate(lap_normal, lv_points)
co_rv_normal = (sv_rv_normal * hr_normal) / 1000.0
co_lv_normal = (sv_lv_normal * hr_normal) / 1000.0

print(f"\nAt normal HR={hr_normal:.0f} bpm:")
print(f"  RAP = {rap_normal:.1f} mmHg → SV_RV = {sv_rv_normal:.1f} ml → CO_RV = {co_rv_normal:.2f} L/min")
print(f"  LAP = {lap_normal:.1f} mmHg → SV_LV = {sv_lv_normal:.1f} ml → CO_LV = {co_lv_normal:.2f} L/min")

if abs(co_rv_normal - co_lv_normal) < 0.5:
    print(f"  ✅ Balanced: CO_RV ≈ CO_LV (diff = {abs(co_rv_normal - co_lv_normal):.2f} L/min)")
else:
    print(f"  ⚠️  UNBALANCED! CO_RV vs CO_LV diff = {abs(co_rv_normal - co_lv_normal):.2f} L/min")

print("\n" + "="*100)

# ========== PLOTTA ==========
fig, axes = plt.subplots(1, 3, figsize=(18, 5))

# Plot 1: RV CO vs RAP
ax1 = axes[0]
for hr, color in zip(hr_values, colors):
    rv_sv = [evaluate(p, rv_points) for p in preload_range]
    co = (np.array(rv_sv) * hr) / 1000.0
    ax1.plot(preload_range, co, color=color, linewidth=2, label=f'HR={hr}')
ax1.set_xlabel('RAP (mmHg)', fontsize=12)
ax1.set_ylabel('Cardiac Output (L/min)', fontsize=12)
ax1.set_title('RV: CO vs RAP', fontsize=14)
ax1.legend(loc='upper left', fontsize=9)
ax1.grid(True, alpha=0.3)
ax1.set_xlim(0, 20)
ax1.set_ylim(0, 8)
ax1.axhline(y=4, color='green', linestyle='--', alpha=0.5)
ax1.axhline(y=8, color='green', linestyle='--', alpha=0.5)
ax1.axvspan(2, 8, alpha=0.1, color='blue')

# Plot 2: LV CO vs LAP
ax2 = axes[1]
for hr, color in zip(hr_values, colors):
    lv_sv = [evaluate(p, lv_points) for p in preload_range]
    co = (np.array(lv_sv) * hr) / 1000.0
    ax2.plot(preload_range, co, color=color, linewidth=2, label=f'HR={hr}')
ax2.set_xlabel('LAP (mmHg)', fontsize=12)
ax2.set_ylabel('Cardiac Output (L/min)', fontsize=12)
ax2.set_title('LV: CO vs LAP', fontsize=14)
ax2.legend(loc='upper left', fontsize=9)
ax2.grid(True, alpha=0.3)
ax2.set_xlim(0, 25)
ax2.set_ylim(0, 14)
ax2.axhline(y=4, color='green', linestyle='--', alpha=0.5)
ax2.axhline(y=8, color='green', linestyle='--', alpha=0.5)
ax2.axvspan(5, 12, alpha=0.1, color='red')

# Plot 3: RV + LV @ HR=72
ax3 = axes[2]
hr_normal = 72
rv_sv = [evaluate(p, rv_points) for p in preload_range]
lv_sv = [evaluate(p, lv_points) for p in preload_range]
co_rv = (np.array(rv_sv) * hr_normal) / 1000.0
co_lv = (np.array(lv_sv) * hr_normal) / 1000.0
ax3.plot(preload_range, co_rv, 'b-', linewidth=3, label='RV (RAP)')
ax3.plot(preload_range, co_lv, 'r-', linewidth=3, label='LV (LAP)')
ax3.axvline(x=6, color='blue', linestyle=':', alpha=0.7)
ax3.axvline(x=10, color='red', linestyle=':', alpha=0.7)
ax3.scatter([6], [co_rv_normal], color='blue', s=100, zorder=5)
ax3.scatter([10], [co_lv_normal], color='red', s=100, zorder=5)
ax3.set_xlabel('Preload (mmHg)', fontsize=12)
ax3.set_ylabel('Cardiac Output (L/min)', fontsize=12)
ax3.set_title(f'RV vs LV @ HR={hr_normal} bpm', fontsize=14)
ax3.legend(loc='upper left', fontsize=9)
ax3.grid(True, alpha=0.3)
ax3.set_xlim(0, 25)
ax3.set_ylim(0, 10)
ax3.axhline(y=4, color='green', linestyle='--', alpha=0.5)
ax3.axhline(y=8, color='green', linestyle='--', alpha=0.5)

plt.tight_layout()
plt.savefig('/workspaces/HeartController/scripts/starling_curves.png', dpi=150, bbox_inches='tight')
plt.show()

print("\n✅ Plot saved to: /workspaces/HeartController/scripts/starling_curves.png")