import numpy as np
import matplotlib.pyplot as plt


def explain_rank_1():
    print("=== RANK 1 MATRIX DEMONSTRATION ===\n")

    # 1. Create two vectors
    u = np.array([1, 2])  # Defines the output line direction
    v = np.array([3, 4])  # Defines how input is projected

    # 2. Create Rank 1 Matrix using Outer Product (u * v.T)
    rank_1_matrix = np.outer(u, v)

    print(f"Vector u: {u}")
    print(f"Vector v: {v}")
    print("\nRank 1 Matrix (u * v^T):")
    print(rank_1_matrix)

    # 3. Verify Properties
    rank = np.linalg.matrix_rank(rank_1_matrix)
    print(f"\nCalculated NumPy Rank: {rank}")

    print("Notice: Row 2 [3, 8] is exactly 2x Row 1 [3, 4].")

    # ==========================================
    # VISUALIZATION
    # ==========================================

    # Generate 100 random points in a 2D square (x: -1 to 1, y: -1 to 1)
    # Shape: (2, 100)
    points = np.random.uniform(-1, 1, (2, 500))

    # Apply the Rank 1 Matrix to all points
    transformed_points = np.dot(rank_1_matrix, points)

    # Plotting
    fig, axes = plt.subplots(1, 2, figsize=(12, 5))

    # Plot 1: Original Data
    axes[0].scatter(points[0, :], points[1, :], alpha=0.6, color='blue')
    axes[0].set_title("Original Data (Rank 2 / Full Rank)")
    axes[0].set_xlim(-5, 5)
    axes[0].set_ylim(-5, 5)
    axes[0].grid(True)
    axes[0].axhline(0, color='black', lw=1)
    axes[0].axvline(0, color='black', lw=1)

    # Plot 2: Transformed Data
    axes[1].scatter(transformed_points[0, :], transformed_points[1, :], alpha=0.6, color='red')

    # Draw the vector u to show the direction
    # The line will follow the slope of u (y = 2x)
    x_vals = np.linspace(-2, 2, 10)
    y_vals = (u[1] / u[0]) * x_vals
    axes[1].plot(x_vals, y_vals, 'k--', label=f'Direction of u ({u[0]},{u[1]})')

    axes[1].set_title("After Rank 1 Transformation\n(Collapsed to 1D Line)")
    axes[1].set_xlim(-5, 5)
    axes[1].set_ylim(-5, 5)
    axes[1].grid(True)
    axes[1].axhline(0, color='black', lw=1)
    axes[1].axvline(0, color='black', lw=1)
    axes[1].legend()

    plt.tight_layout()
    plt.show()


if __name__ == "__main__":
    explain_rank_1()
