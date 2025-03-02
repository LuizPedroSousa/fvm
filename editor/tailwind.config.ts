import { fontFamily } from "tailwindcss/defaultTheme";
import type { Config } from "tailwindcss";

const config: Config = {
  darkMode: "selector",
  content: ["./src/**/*.{html,js,svelte,ts}"],
  safelist: ["dark"],
  theme: {
    container: {
      center: true,
      padding: "2rem",
      screens: {
        "2xl": "1400px",
      },
    },
    extend: {
      colors: {
        border: "hsl(var(--border) / <alpha-value>)",
        input: "hsl(var(--input) / <alpha-value>)",
        ring: "hsl(var(--ring) / <alpha-value>)",
        gray: {
          300: "var(--gray-300)",
        },
        handle: "var(--handle)",
        background: "var(--background)",
        foreground: "hsl(var(--foreground) / <alpha-value>)",
        'cinder': {
          '50': '#f6f5fa',
          '100': '#ebeaf4',
          '200': '#d2d1e6',
          '300': '#aba9d0',
          '400': '#7c7bb5',
          '500': '#5c5a9d',
          '600': '#4a4782',
          '700': '#3d3a6a',
          '800': '#353359',
          '900': '#302e4c',
          '950': '#0f0e17',
        },
        'cabaret': {
          '50': '#fdf2f7',
          '100': '#fde6f1',
          '200': '#fdcde3',
          '300': '#fca5cc',
          '400': '#fa6ca9',
          '500': '#f34188',
          '600': '#e53170',
          '700': '#c5114b',
          '800': '#a3113e',
          '900': '#881337',
          '950': '#53041c',
        },
        'sunset-orange': {
          '50': '#fef3f2',
          '100': '#fee5e2',
          '200': '#ffcfc9',
          '300': '#fdaea4',
          '400': '#fa7f6f',
          '500': '#f25f4c',
          '600': '#de3924',
          '700': '#bb2c1a',
          '800': '#9b2819',
          '900': '#80271c',
          '950': '#461009',
        },
        primary: {
          DEFAULT: "hsl(var(--primary) / <alpha-value>)",
          foreground: "hsl(var(--primary-foreground) / <alpha-value>)",
        },
        secondary: {
          DEFAULT: "hsl(var(--secondary) / <alpha-value>)",
          foreground: "hsl(var(--secondary-foreground) / <alpha-value>)",
        },
        destructive: {
          DEFAULT: "hsl(var(--destructive) / <alpha-value>)",
          foreground: "hsl(var(--destructive-foreground) / <alpha-value>)",
        },
        muted: {
          DEFAULT: "hsl(var(--muted) / <alpha-value>)",
          foreground: "hsl(var(--muted-foreground) / <alpha-value>)",
        },
        accent: {
          DEFAULT: "hsl(var(--accent) / <alpha-value>)",
          foreground: "hsl(var(--accent-foreground) / <alpha-value>)",
        },
        popover: {
          DEFAULT: "hsl(var(--popover) / <alpha-value>)",
          foreground: "hsl(var(--popover-foreground) / <alpha-value>)",
        },
        card: {
          DEFAULT: "hsl(var(--card) / <alpha-value>)",
          foreground: "hsl(var(--card-foreground) / <alpha-value>)",
        },
      },
      borderRadius: {
        lg: "var(--radius)",
        md: "calc(var(--radius) - 2px)",
        sm: "calc(var(--radius) - 4px)",
      },
      fontFamily: {
        sans: [...fontFamily.sans],
      },
    },
  },
};

export default config;
