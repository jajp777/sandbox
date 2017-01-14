﻿using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Numerics;

namespace Assignements
{
    public static class KaratsubaMultiplication
    {
        public static string Multiply(string l, string r)
        {
            if ((l.Length == 1) && (r.Length == 1))
            {
                return ((l[0] - 48) * (r[0] - 48)).ToString();
            }
            else
            {
                l = l.TrimStart('0');
                r = r.TrimStart('0');

                if (l.Length == 0 || r.Length == 0)
                {
                    return "0";
                }
                else if (l == "1")
                {
                    return r;
                }
                else if (r == "1")
                {
                    return l;
                }
                else if ((l.Length == 1) && (r.Length == 1))
                {
                    return ((l[0] - 48) * (r[0] - 48)).ToString();
                }

                int maxlength = Math.Max(l.Length, r.Length);

                if (l.Length > r.Length)
                {
                    r = r.PadLeft(l.Length, '0');
                }
                else if (l.Length < r.Length)
                {
                    l = l.PadLeft(r.Length, '0');
                }

                if (l.Length % 2 != 0)
                {
                    l = l.PadLeft(l.Length + (l.Length % 2), '0');
                    r = r.PadLeft(r.Length + (r.Length % 2), '0');
                }

                var a = l.Substring(0, l.Length / 2);
                var b = l.Substring(l.Length / 2, l.Length / 2);

                var c = r.Substring(0, l.Length / 2);
                var d = r.Substring(l.Length / 2, l.Length / 2);

                string ac = Multiply(a, c);
                //string ad = Multiply(a, d);
                //string bc = Multiply(b, c);
                string bd = Multiply(b, d);

                var aplusb = Sum1(a, b);
                var cplusd = Sum1(c, d);
                var adbc = Minus(Minus(Multiply(aplusb, cplusd), ac), bd);

                //string adbc = Sum1(ad, bc);

                var p1 = (BigInteger.Pow(10, l.Length) * BigInteger.Parse(ac)).ToString();
                //var p1 = ac + new string('0', ll);
                var p2 = (BigInteger.Pow(10, l.Length / 2) * BigInteger.Parse(adbc)).ToString();

                //var result = Sum(Sum(bd, p2), p1);

                var result = ((BigInteger.Parse(bd) + BigInteger.Parse(p2)) + BigInteger.Parse(p1)).ToString();

                Console.WriteLine($"{l} * {r} = {result} \n    = [{a}.10^{maxlength - 1}+{b}]*[{c}.10^{l.Length / 2}+{d}]\n    = {ac}.10^{l.Length}+(({a}+{b})*({c}+{d})-{ac}-{bd}).10^{l.Length / 2}+{bd}\n    = {p1} + {p2} + {bd}");

                return result;
            }
        }

        public static string Sum1(string l, string r)
        {
            if (l.Length == 1 && r.Length == 1)
            {
                return (int.Parse(l) + int.Parse(r)).ToString();
            }
            else
            {
                var li = l.Length - 1;
                var ri = r.Length - 1;

                var maxlength = Math.Max(l.Length, r.Length);
                var result = new char[maxlength + 1];

                int carriage = 0;
                for (int i = maxlength; i > 0; i--)
                {
                    int lc = 0;
                    if (li >= 0)
                    {
                        lc = (int)l[li] - 48;
                        li--;
                    }

                    int rc = 0;
                    if (ri >= 0)
                    {
                        rc = (int)r[ri] - 48;
                        ri--;
                    }

                    var re = lc + rc + carriage;

                    if (re >= 10)
                    {
                        re = re - 10;
                        carriage = 1;
                    }
                    else
                    {
                        carriage = 0;
                    }

                    result[i] = (char)(re + 48);
                }

                if (carriage == 0)
                {
                    return new string(result.Skip(1).ToArray());
                }
                else
                {
                    result[0] = '1';
                    return new string(result.ToArray());
                }
            }
        }

        public static string Sum2(string l, string r)
        {
            if (l.Length > r.Length)
            {
                r = r.PadLeft(l.Length, '0');
            }
            else if (l.Length < r.Length)
            {
                l = l.PadLeft(r.Length, '0');
            }

            var result = new char[l.Length + 1];

            int carriage = 0;
            for (int i = l.Length - 1; i >= 0; --i)
            {
                var s = ((char)l[i] - 48) + ((char)r[i] - 48) + carriage;
                if (s > 9)
                {
                    carriage = 1;
                    s = s - 10;
                }
                else
                {
                    carriage = 0;
                }

                result[i + 1] = (char)(s + 48);
            }

            if (carriage == 0)
            {
                return new string(result.Skip(1).ToArray());
            }
            else
            {
                result[0] = '1';
                return new string(result.ToArray());
            }
        }

        public static string Minus(string l, string r)
        {
            return (BigInteger.Parse(l) - BigInteger.Parse(r)).ToString();
        }
    }

    [TestClass]
    public class UnitTest1
    {
        [TestMethod]
        public void Sum()
        {
            var data = new[]
            {
                //new [] { "1", "2" },
                new [] { "10", "2" },
                new [] { "100", "200" },
                new [] { "1000", "2000" },
                new [] { "60000", "60000" },
                //new [] { "100000", "200000" },
                //new [] { "10000", "20000" },
                //new [] { "10000000", "20000000" },
                //new [] { "3141592653589793238462643383279502884197169399375105820974944592", "2718281828459045235360287471352662497757247093699959574966967627" }
            };

            foreach (var item in data)
            {
                var result = System.Numerics.BigInteger.Parse(item[0]) + System.Numerics.BigInteger.Parse(item[1]);
                Assert.AreEqual(result.ToString(),
                    KaratsubaMultiplication.Sum1(item[0], item[1]));

                Console.WriteLine("---------------------------------");
                Console.WriteLine(result.ToString());
                Console.WriteLine("---------------------------------");
            }
        }

        [TestMethod]
        public void SumRandom1()
        {
            var random = new Random();

            foreach (var item in Enumerable.Range(0, 100000))
            {
                var l = random.Next().ToString();
                var r = random.Next().ToString();
                var result = BigInteger.Parse(l) + BigInteger.Parse(r);
                Assert.AreEqual(result.ToString(),
                    KaratsubaMultiplication.Sum1(l, r));

                Console.WriteLine("---------------------------------");
                Console.WriteLine(result.ToString());
                Console.WriteLine("---------------------------------");
            }
        }

        [TestMethod]
        public void SumRandom2()
        {
            var random = new Random();

            foreach (var item in Enumerable.Range(0, 100000))
            {
                var l = random.Next().ToString();
                var r = random.Next().ToString();
                var result = BigInteger.Parse(l) + BigInteger.Parse(r);
                Assert.AreEqual(result.ToString(),
                    KaratsubaMultiplication.Sum2(l, r));

                Console.WriteLine("---------------------------------");
                Console.WriteLine(result.ToString());
                Console.WriteLine("---------------------------------");
            }
        }

        [TestMethod]
        public void TestMethod1()
        {
            var data = new[]
            {
                //new [] { "1", "2" },
                //new [] { "10", "20" },
                //new [] { "100", "200" },
                //new [] { "1000", "2000" },
                //new [] { "10000", "20000" },
                //new [] { "100000", "200000" },
                //new [] { "10000", "20000" },
                //new [] { "10000000", "20000000" },
                //new [] { "07", "0142" },
                new [] { "0010", "0486" }  
                //new [] { "3141592653589793238462643383279502884197169399375105820974944592", "2718281828459045235360287471352662497757247093699959574966967627" }
            };

            foreach (var item in data)
            {
                var result = System.Numerics.BigInteger.Parse(item[0]) * System.Numerics.BigInteger.Parse(item[1]);
                Assert.AreEqual(result.ToString(),
                    KaratsubaMultiplication.Multiply(item[0], item[1]));

                Console.WriteLine("---------------------------------");
                Console.WriteLine(result.ToString());
                Console.WriteLine("---------------------------------");
            }
        }

        [TestMethod]
        public void MulRandom()
        {
            var random = new Random();

            foreach (var item in Enumerable.Range(0, 2000))
            {
                var l = random.Next().ToString();
                var r = random.Next().ToString();
                var result = BigInteger.Parse(l) * BigInteger.Parse(r);
                Assert.AreEqual(result.ToString(),
                    KaratsubaMultiplication.Multiply(l, r));

                Console.WriteLine("---------------------------------");
                Console.WriteLine(result.ToString());
                Console.WriteLine("---------------------------------");
            }
        }
    }

    [TestClass]
    public class Week02Challenges
    {
        public class Comparer
        {
            public int Count { get; set; }

            public int Compare(int l, int r)
            {
                Count++;
                return l.CompareTo(r);
            }
        }

        public class SGN2
        {
            public Comparer Comparer;

            public SGN2()
            {
                Comparer = new Comparer();
            }

            public int Get(params int[] numbers)
            {
                var ns = new[] { Math.Min(numbers[0], numbers[1]), Math.Max(numbers[0], numbers[1]) };

                for (int i = 2; i < numbers.Length; ++i)
                {
                    if (Comparer.Compare(numbers[i], ns[1]) > 0)
                    {
                        ns[0] = ns[1];
                        ns[1] = numbers[i];
                    }
                    else if (Comparer.Compare(numbers[i], ns[0]) > 0)
                    {
                        ns[0] = numbers[i];
                    }
                }

                return ns[0];
            }
        }

        public class SecondGreatestNumber
        {
            public Comparer Comparer;

            public SecondGreatestNumber()
            {
                Comparer = new Comparer();
            }

            public int Get(params int[] numbers)
            {
                return _Get(numbers)[0];
            }

            public int[] _Get(params int[] numbers)
            {
                if (numbers.Length == 2)
                {
                    Array.Sort(numbers);
                    return numbers;
                }
                else
                {
                    int[] larger = new int[2];

                    int n = numbers.Length;
                    var l = _Get(numbers.Take(n / 2).ToArray());
                    var r = _Get(numbers.Skip(n / 2).ToArray());

                    int largeri = 0;
                    int li = 0;
                    int ri = 0;
                    for (int i = 0; i < 4 && largeri < 2; i++)
                    {
                        if (Comparer.Compare(l[li], r[li]) > 0)
                        {
                            larger[largeri] = l[li];
                            li++;
                        }
                        else
                        {
                            larger[largeri] = r[ri];
                            ri++;
                        }
                        largeri++;
                    }

                    return larger;
                }
            }
        }


        private static int[] GetArray(int size)
        {
            var array = new int[size];

            var r = new Random();
            for (int i = 0; i < size; i++)
            {
                array[i] = r.Next();
            }

            return array;
        }

        private static void ShowComparisons(int[] array)
        {
            var sgn = new SecondGreatestNumber();
            var n = sgn.Get(array);
            //Assert.AreEqual(3, n);
            Console.Write($"{sgn.Comparer.Count} - ");

            var SGN2 = new SGN2();
            SGN2.Get(array);

            Console.WriteLine(SGN2.Comparer.Count);
        }

        /// <summary>
        ///You are given as input an unsorted array of n distinct numbers,
        ///where n is a power of 2. Give an algorithm that identifies
        ///the second-largest number in the array, and
        ///that uses at most n+log2n−2 comparisons.
        /// </summary>
        [TestMethod]        
        public void GetSecondBiggest()
        {
            for (int i = 1; i < 10; ++i)
            {
                int size = (int)Math.Pow(2, i);
                Console.Write($"Size: {size} - {size + (int)Math.Log(size, 2) - 2} - ");
                ShowComparisons(GetArray(size));
            }
        }

        /// <summary>
        /// You are a given a unimodal array of n distinct elements,
        /// meaning that its entries are in increasing order up
        /// until its maximum element, after which its elements are
        /// in decreasing order. Give an algorithm to compute the maximum
        /// element that runs in O(log n) time.
        /// </summary>
        [TestMethod]
        public void FindUnimodalBiggest()
        {
            var array = new[] { 1, 2, 3, 4, 5, 6, 5, 4 };
            var big = GetUnimodalBiggest(array);

            Assert.AreEqual(6, big);

            //var alg = new MasterMethod<int[]>();
            //alg.DividerSize = 2;
            //alg.AddDivider(x => x.Take(x.Length / 2).ToArray());
            ////alg.AddDivider(x => x.Skip(x.Length / 2).ToArray());
            //alg.ConstantConquer((l, r) =>
            //{
            //    return 0;
            //});

            //Console.WriteLine(alg.Complexity);
        }

        private int GetUnimodalBiggest(int[] array)
        {
            int n = array.Length;

            if (n <= 2)
            {
                return Math.Max(array[0], array[1]);
            }
            else
            {
                var l = array.Take(n / 2).ToArray();
                var r = array.Skip(n / 2).ToArray();

                var llast = l.Last();
                var rfirst = r.First();

                if (rfirst > llast)
                {
                    return GetUnimodalBiggest(r);
                }
                else //if (rfirst < llast)
                {
                    return GetUnimodalBiggest(l);
                }
            }
        }
    }

    public class MasterMethod<T>
    {
        List<Func<T, T>> Dividers = new List<Func<T, T>>();
        int D = 0;

        public int DividerSize { get; set; }

        public string Complexity
        {
            get
            {
                int a = Dividers.Count;
                int b = DividerSize;
                int d = D;

                if (a == (int)Math.Pow(b, d))
                {
                    if (d == 0)
                    {
                        return "O(log(n))";
                    }
                    else
                    {
                        return $"O(n^{d}*log(n))";
                    }
                }
                else if (a > (int)Math.Pow(b, d))
                {
                    var logba = (int)Math.Log(a, b);
                    return $"O(n^{logba})";
                }
                else //if (a < (int)Math.Pow(d, b))
                {
                    return $"O(n^{d})";
                }
            }
        }

        public void AddDivider(Func<T, T> divider)
        {
            Dividers.Add(divider);
        }

        internal void ConstantConquer(Func<object, object, object> p)
        {
            D = 0;
        }
    }
}
