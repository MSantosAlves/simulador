Simulador [B3]

Python:

- plotly
- pandas
- dash
- tkinter (sudo apt-get install python3-tk)

Parse Data:

1. Adicionar os arquivos RAW ao diretorio /raw
2. Rodar o script 00_split_data_into_chunks.py
3. Apagar primeira linha do primeiro arquivo e ultima linha do ultimo arquivo (???)
4. Rodar os scripts (em sequência)
    - 00_split_data_into_chunks.py
    - 01_chunk_data_analysis.py
    - 02_compile_data_information.py
    - 05_index_stock_by_file.py

Run Server:
```
mkdir build-dir

cd build-dir

cmake ..

make
```