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

        public double v_ref = 3300;






        public void run()
        {
            while (this.mode == Paint_mode.Mode.GENERATOR)
            {
                Console.WriteLine("gen bezi " + this.signal.Length);
                generatorPane.CurveList.Clear();

                generatorPane.YAxis.Scale.MaxAuto = false;
                generatorPane.YAxis.Scale.MinAuto = false;

                generatorPane.XAxis.Scale.MaxAuto = false;
                generatorPane.XAxis.Scale.MinAuto = false;

                generatorPane.XAxis.Scale.Max = 1.0/frequency;
                generatorPane.XAxis.Scale.Min = 0;

                generatorPane.YAxis.Scale.Max = v_ref/1000;
                generatorPane.YAxis.Scale.Min = 0;

                for (int i = 0; i < buffLenght; i++) {
                    this.time[i] = (double)i/frequency/buffLenght;
                    if (sigType == SignalType.SINE) {
                        this.signal[i] = Math.Sin(time[i]*frequency*2*Math.PI)*amplitude/v_ref*v_ref/1000 + offset/1000;
                        if (signal[i] < 0) {
                            signal[i] = 0;
                        }
                        else if (signal[i] > v_ref/1000) {
                            signal[i] = v_ref/1000;
                        }
                        
                    }
                }

                LineItem curve = generatorPane.AddCurve("", time, signal, Color.Red, SymbolType.Diamond);
                curve.Line.IsSmooth = false;
                curve.Line.IsOptimizedDraw = true;
                curve.Symbol.Size = 0;
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
