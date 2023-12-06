import sys
from PyQt5.QtWidgets import QApplication, QMainWindow, QVBoxLayout, QWidget, QPushButton, QFileDialog
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
import matplotlib.pyplot as plt
import pandas as pd

class WaterFlowApp(QMainWindow):
    def __init__(self):
        super().__init__()

        self.setWindowTitle('Visulaisasi data water flow meter')
        self.setGeometry(100, 100, 800, 600)

        central_widget = QWidget(self)
        self.setCentralWidget(central_widget)

        layout = QVBoxLayout(central_widget)

        self.canvas = FigureCanvas(plt.Figure(figsize=(6, 4)))
        layout.addWidget(self.canvas)

        self.load_button = QPushButton('Pilih File', self)
        self.load_button.clicked.connect(self.load_data_and_plot)
        layout.addWidget(self.load_button)

        self.load_data_and_plot()

    def load_data_and_plot(self):
        file_dialog = QFileDialog()
        file_path, _ = file_dialog.getOpenFileName(self, 'Pilih File Pengukuran', '', 'CSV Files (*.csv)')

        if not file_path:
            return

        data = pd.read_csv(file_path)

        # reset subplot
        self.canvas.figure.clear()

        # Buat subplot baru
        ax = self.canvas.figure.add_subplot(111)

        # Plot the new data
        ax.plot(data['Tanggal Waktu'], data[' Total Pembayaran (Rp)'], label=' Total Pembayaran (Rp)')
        ax.set_xlabel('Tanggal Waktu')
        ax.tick_params(axis='x', rotation=45)
        ax.set_ylabel(' Total Pembayaran (Rp)')
        ax.set_title('Water Flow Data')
        ax.legend()

        self.canvas.draw()

def main():
    app = QApplication(sys.argv)
    window = WaterFlowApp()
    window.show()
    sys.exit(app.exec_())

if __name__ == '__main__':
    main()
