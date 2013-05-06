﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ZedGraph;
using System.Drawing;
using System.Threading;

namespace PTO_PC_APP
{
    class Scope_thread
    {
        //scope promenne
        public GraphPane scopePane;

        public Paint_mode.Mode mode;
        //Stopwatch stopWatch;


        public int trig_level = 0;
        public int pretrig = 100;
        public int buffLenght = 0;
        public double scale = 1;
        public double horPosition = 0.5;

        public double verPosition = 0;
        public double accuracy = 0;  //prepocet z bitu na volty
        public double range = 5;
        public int maxRange = 5; //V;

        public bool horCurEN = false;
        public double horCursorA = 0;
        public double horCursorB = 0;
        public string voltA = "";
        public string voltB = "";
        public string voltDif = "";

        public bool verCurEN = false;
        public double verCursorA = 0;
        public double verCursorB = 0;
        public string UA = "";
        public string UB = "";
        public string DiffU = "";
        public string timeA = "";
        public string timeB = "";
        public string timeDif = "";
        public string freq = "";
        public int samplingfreq = 10000;
        public double[] signal;
        public double[] time;
        public double maxTime = 0;

        private bool invalid=true;
        private bool lockPane = false;
     
        public void Scope_thready(int smp) {
            this.samplingfreq = smp;
        }

        public void update_time_base() {
            generate_time_base(samplingfreq, buffLenght);
        }

        //vypocet casove zakladny
        private void generate_time_base(int sampling, int lenght)
        {
            for (int i = 0; i < lenght; i++)
            {
                time[i] = (double)(i) / sampling;
            }

            maxTime = (double)(lenght) / sampling;
        }

        public void new_data(byte[] d)
        {
            tic();
            generate_time_base(samplingfreq, buffLenght);
            for (int i = 0; i < buffLenght; i++)
            {
                signal[i] = accuracy * d[i] / 1000;
            }
            toc("new data processed");
        }

        public void run()
        {
            while (this.mode == Paint_mode.Mode.SCOPE)
            {
                //osy grafu
                lockPane = true;
                scopePane.YAxis.Scale.MaxAuto = false;
                scopePane.YAxis.Scale.MinAuto = false;

                scopePane.XAxis.Scale.MaxAuto = false;
                scopePane.XAxis.Scale.MinAuto = false;

                double interval = scale * maxTime;
                double posmin = (interval / 2);
                double posScale = (maxTime - interval) / maxTime;

                double maxX = (maxTime) * horPosition * posScale + posmin + interval / 2;
                double minX = (maxTime) * horPosition * posScale + posmin - interval / 2;

                interval = range;
                posmin = (interval / 2);
                posScale = (maxRange - interval) / maxRange;

                double maxY = maxRange * verPosition * posScale + posmin + interval / 2;
                double minY = maxRange * verPosition * posScale + posmin - interval / 2;

                scopePane.XAxis.Scale.Max = maxX;
                scopePane.XAxis.Scale.Min = minX;

                scopePane.YAxis.Scale.Max = maxY;
                scopePane.YAxis.Scale.Min = minY;

                //vykresleni prubehu
                scopePane.CurveList.Clear();
                LineItem curve = scopePane.AddCurve("", time, signal, Color.Red, SymbolType.Diamond);
                curve.Symbol.Size = 2;
                curve.Line.IsSmooth = true;
                curve.Line.SmoothTension = 0.5F;
                curve.Line.IsOptimizedDraw = true;


                //zoom position
                PointPairList list1 = new PointPairList();
                list1.Add((maxTime) * horPosition, maxY);
                curve = scopePane.AddCurve("", list1, Color.Red, SymbolType.TriangleDown);
                curve.Symbol.Size = 15;
                curve.Symbol.Fill.Color = Color.Red;
                curve.Symbol.Fill.IsVisible = true;

                //trigger time
                list1 = new PointPairList();
                list1.Add((maxTime) * pretrig / 100, maxY);
                curve = scopePane.AddCurve("", list1, Color.Blue, SymbolType.TriangleDown);
                curve.Symbol.Size = 20;
                curve.Symbol.Fill.Color = Color.Blue;
                curve.Symbol.Fill.IsVisible = true;

                //triggerlevel
                list1 = new PointPairList();
                list1.Add(minX, trig_level / 16 * accuracy / 1000);
                curve = scopePane.AddCurve("", list1, Color.Green, SymbolType.Diamond);
                curve.Symbol.Size = 15;
                curve.Symbol.Fill.Color = Color.Green;
                curve.Symbol.Fill.IsVisible = true;

                //kurzory
                if (horCurEN)
                {
                    double uA = horCursorA * maxY + (1 - horCursorA) * minY;
                    double uB = horCursorB * maxY + (1 - horCursorB) * minY;
                    double ud = uA - uB;
                    this.voltDif = "dU " + (Math.Round(ud * 1000, 2)).ToString() + " mV";
                    this.voltA = "U " + (Math.Round(uA * 1000, 2)).ToString() + " mV";
                    this.voltB = "U " + (Math.Round(uB * 1000, 2)).ToString() + " mV";

                    list1 = new PointPairList();
                    list1.Add(minX, uA);
                    list1.Add(maxX, uA);
                    curve = scopePane.AddCurve("", list1, Color.DarkBlue, SymbolType.HDash);
                    curve.Symbol.Size = 0;
                    curve.Line.IsSmooth = true;


                    list1 = new PointPairList();
                    list1.Add(minX, uB);
                    list1.Add(maxX, uB);
                    curve = scopePane.AddCurve("", list1, Color.DarkBlue, SymbolType.HDash);
                    curve.Symbol.Size = 0;
                    curve.Line.IsSmooth = true;
                }

                if (verCurEN)
                {
                    //vypocet casu
                    double tA = verCursorA * maxX + (1 - verCursorA) * minX;
                    double tB = verCursorB * maxX + (1 - verCursorB) * minX;
                    int indexUA = (int)(tA / maxTime * buffLenght);
                    int indexUB = (int)(tB / maxTime * buffLenght);
                    double VA = 0;
                    double VB = 0;

                    //vypocet linearni interpolace napeti kurzoru
                    if (indexUA < buffLenght - 1)
                    {
                        VA = signal[indexUA] + (tA - time[indexUA]) / (time[indexUA + 1] - time[indexUA]) * (signal[indexUA + 1] - signal[indexUA]);
                    }
                    else
                    {
                        VA = signal[indexUA - 1];
                    }

                    if (indexUB < buffLenght - 1)
                    {
                        VB = signal[indexUB] + (tB - time[indexUB]) / (time[indexUB + 1] - time[indexUB]) * (signal[indexUB + 1] - signal[indexUB]);
                    }
                    else
                    {
                        VB = signal[indexUB - 1];
                    }
                    double td = tA - tB;
                    double ud = VA - VB;
                    double f = Math.Abs(1 / td);

                    //formatovani stringu
                    this.timeDif = "dt " + (Math.Round(td * 1000, 3)).ToString() + " ms";
                    this.timeA = "t " + (Math.Round(tA * 1000, 3)).ToString() + " ms";
                    this.timeB = "t " + (Math.Round(tB * 1000, 3)).ToString() + " ms";
                    if (Double.IsInfinity(f))
                    {
                        this.freq = "f Inf";
                    }
                    else if (f >= 1000000)
                    {
                        this.freq = "f " + (Math.Round(f / 1000000, 3)).ToString() + " MHz";
                    }
                    else if (f >= 1000)
                    {
                        this.freq = "f " + (Math.Round(f / 1000, 3)).ToString() + " kHz";
                    }
                    else
                    {
                        this.freq = "f " + (Math.Round(f, 3)).ToString() + " Hz";
                    }

                    if (VA >= 1)
                    {
                        this.UA = "U " + (Math.Round(VA, 2)).ToString() + " V";
                    }
                    else
                    {
                        this.UA = "U " + (Math.Round(VA * 1000, 2)).ToString() + " mV";
                    }

                    if (VB >= 1)
                    {
                        this.UB = "U " + (Math.Round(VB, 2)).ToString() + " V";
                    }
                    else
                    {
                        this.UB = "U " + (Math.Round(VB * 1000, 2)).ToString() + " mV";
                    }

                    if (ud >= 1)
                    {
                        this.DiffU = "U " + (Math.Round(ud, 2)).ToString() + " V";
                    }
                    else
                    {
                        this.DiffU = "U " + (Math.Round(ud * 1000, 2)).ToString() + " mV";
                    }

                    //vykresleni kurzoru
                    list1 = new PointPairList();
                    list1.Add(tA, minY);
                    list1.Add(tA, maxY);
                    curve = scopePane.AddCurve("", list1, Color.DarkBlue, SymbolType.HDash);
                    curve.Symbol.Size = 0;
                    curve.Line.IsSmooth = true;

                    list1 = new PointPairList();
                    list1.Add(tB, minY);
                    list1.Add(tB, maxY);
                    curve = scopePane.AddCurve("", list1, Color.DarkBlue, SymbolType.HDash);
                    curve.Symbol.Size = 0;
                    curve.Line.IsSmooth = true;

                }
                //zedGraphControl_scope.AxisChange();
                //zedGraphControl_scope.Invalidate();
                //Invalidate();
                scopePane.AxisChange();
                lockPane = false;
                invalid = true;
                Thread.Sleep(50);
            }
        }


        public bool is_locked(){
            return lockPane;
        }
        public bool is_invalid(){
            if (invalid)
            {
                invalid = false;
                return true;
            }
            else
            {
                return false;
            }
        }

        public void update_mode(Paint_mode.Mode m) {
            this.mode = m;
        }

        private void tic()
        {
            //stopWatch = new Stopwatch();
            //stopWatch.Start();
        }

        private void toc(string s)
        {
            //stopWatch.Stop();
            //Console.WriteLine(s + " " + stopWatch.ElapsedMilliseconds.ToString() + "ms");
        }
    }
}
