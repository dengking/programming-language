# pandas



## pandas create a dataframe with column index and row index

In pandas, a DataFrame is a two-dimensional, size-mutable, and potentially heterogeneous tabular data structure with labeled axes (rows and columns). When you create a DataFrame, you can specify both the column and row indices (labels) using the `columns` and `index` parameters, respectively.

Here's an example of how to create a DataFrame with specific column and row indices:

```python
import pandas as pd

# Define the data as a list of lists or a dictionary of lists
data = {
    'Column1': [1, 2, 3],
    'Column2': [4, 5, 6],
    'Column3': [7, 8, 9]
}

# Define the row index (labels for the rows)
row_index = ['Row1', 'Row2', 'Row3']

# Define the column index (labels for the columns)
column_index = ['Column1', 'Column2', 'Column3']

# Create the DataFrame
df = pd.DataFrame(data, index=row_index, columns=column_index)

print(df)
```

The output will be:

```
      Column1  Column2  Column3
Row1        1        4        7
Row2        2        5        8
Row3        3        6        9
```

In this example, `row_index` is used to label the rows, and `column_index` is used to label the columns. The `data` can be provided as a dictionary where keys are column labels and values are lists of column data, or as a list of lists (or other sequences) where each inner list represents a row of data.

If you provide a list of lists as data, you should also provide the `columns` parameter to label the columns:

```python
# Define the data as a list of lists
data = [
    [1, 4, 7],
    [2, 5, 8],
    [3, 6, 9]
]

# Create the DataFrame with row and column indices
df = pd.DataFrame(data, index=row_index, columns=column_index)

print(df)
```

The output will be the same as the previous example.