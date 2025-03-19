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

        'bunker': {
          '50': '#f7f8fa',
          '100': '#e9ecf2',
          '200': '#d1d6e0',
          '300': '#acb3c3',
          '400': '#838ba3',
          '450': '#747b93',
          '500': '#656b85',
          '550': '#5a5f77',
          '600': '#4f546a',
          '700': '#414659',
          '800': '#383d4d',
          '900': '#2f3441',
          '950': '#242833',
          '1000': '#1a1d26'
        },

        'cabaret': {
          '50': '#fdf3f8',
          '100': '#fde7f2',
          '200': '#fbcfe3',
          '300': '#f9a8cc',
          '400': '#f26ca6',
          '500': '#e93f85',
          '600': '#d92d6d',
          '700': '#b7104b',
          '800': '#94103d',
          '900': '#7b0e34',
          '950': '#48021a'
        },

        'sunset-orange': {
          '50': '#fff8ef',
          '100': '#ffeece',
          '200': '#ffda9c',
          '300': '#ffc166',
          '400': '#ff9b33',
          '500': '#f97b08',
          '600': '#df6507',
          '700': '#b24c09',
          '800': '#8b3b0d',
          '900': '#6f300f',
          '950': '#3e1706'
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
