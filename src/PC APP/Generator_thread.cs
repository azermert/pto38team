using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ZedGraph;
using System.Threading;
using System.Drawing;

namespace PTO_PC_APP
{
    class Generator_thread
    {            
        //gen promenne
        public GraphPane generatorPane;

        public Paint_mode.Mode mode;

        private double[] signal;
        private double[] time;
        private int buffLenght;

        public enum SignalType { SINE,SQUARE,TRIANGLE,RAMP};

        public SignalType sigType= SignalType.SINE;
        public double duty = 0; //%
        public double amplitude = 3000;// mV
        public double frequency = 100; //Hz
        public double offset = 0;//mV






        public void run()
        {
            while (this.mode == Paint_mode.Mode.GENERATOR)
            {
                Console.WriteLine("gen bezi " + this.signal.Length);
                generatorPane.CurveList.Clear();
                for (int i = 0; i < buffLenght; i++) {
                    this.time[i] = (double)i/buffLenght;
                    if (sigType == SignalType.SINE) {
                        this.signal[i] = Math.Sin(time[i]*2*Math.PI) + offset;
                    }
                }

                LineItem curve = generatorPane.AddCurve("", time, signal, Color.Red, SymbolType.Diamond);
                curve.Line.IsSmooth = false;
                curve.Line.IsOptimizedDraw = true;
                generatorPane.AxisChange();
                Thread.Sleep(100);
            }
        }


        public void update_mode(Paint_mode.Mode m)
        {
            this.mode = m;
        }

        internal void set_gen_buff_lenght(int p)
        {
            this.signal = new double[p];
            this.time = new double[p];
            this.buffLenght = p;
        }
    }
}
